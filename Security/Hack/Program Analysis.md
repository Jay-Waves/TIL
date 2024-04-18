|                   | Error exists   | No Error Exists |
| ----------------- | -------------- | --------------- |
| Error Reported    | True Positive  | False Positive  |
| No Error Reported | False Negative | True Negative                |

- Soundness: report all defects (no false negatives)
- Completeness: every reported defect is an actual defect (no flase positives)

### Static vs Dynamic Tradeoffs

- Coverage
	- generalize to addtional traces?
- Soundness
	- every actual data race it reported
- Completeness
	- all reported warnings are actually races
- Overhead
	- run-time slowdown
	- memory footprint
- Programmer Overhead


### White Language

