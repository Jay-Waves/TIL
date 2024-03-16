核心思路还是监控内存访问, 如果有多个线程在同一程序时钟对同一内存地址 (以 8bytes 为单位) 进行访问, 就会警告.  TSan 存在大量误报可能, 所以仅是竞争警告.

| TID(Thread id)             | 16 bits |
| -------------------------- | ------- |
| Scalar Clock               | 42 bits |
| IsWrite                    | 1 bit   |
| Access Szie (1, 2, 4 or 8) | 2 bits  |
| Address Offset (0..7)            | 3 bits        |

see `tsan_rtl.cc`

```cpp
def HandleMemoryAccess(addr, tid, is_write, size, pc):
  shadow_address = MapApplicationToShadow(addr)
  IncrementThreadClock(tid)
  LogEvent(tid, pc);
  new_shadow_word = {tid, CurrentClock(tid), is_write, size, addr & 7}
  store_word = new_shadow_word
  for i in 1..N:
    UpdateOneShadowState(shadow_address, i, new_shadow_word, store_word)
  if store_word:
    # Evict a random Shadow Word
    shadow_address[Random(N)] = store_word  # Atomic
```

```cpp
def UpdateOneShadowState(shadow_address, i, new_shadow_word, store_word):
  idx = (i + new_shadow_word.offset) % N
  old_shadow_word = shadow_address[idx]  # Atomic
  if old_shadow_word == 0: # The old state is empty
    if store_word:
      StoreIfNotYetStored(shadow_address[idx], store_word)
    return
  if AccessedSameRegion(old_shadow_word, new_shadow_word):
    if SameThreads(old_shadow_word, new_shadow_word):
      TODO
    else:  # Different threads
      if not HappensBefore(old_shadow_word, new_shadow_word):
        ReportRace(old_shadow_word, new_shadow_word)
  elif AccessedIntersectingRegions(old_shadow_word, new_shadow_word):
    if not SameThreads(old_shadow_word, new_shadow_word)
      if not HappensBefore(old_shadow_word, new_shadow_word)
        ReportRace(old_shadow_word, new_shadow_word)
  else: # regions did not intersect
    pass # do nothing

def StoreIfNotYetStored(shadow_address, store_word):
  *shadow_address = store_word  # Atomic
  store_word = 0
```