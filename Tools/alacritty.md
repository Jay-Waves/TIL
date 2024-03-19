alacritty 配置文件 `./config/alacritty/alcritty.yml`

```yaml
# Dracula colors for Alacritty

colors:
  # Default colors
  primary:
    background: '0x282a36'
    foreground: '0xf8f8f2'

  # Normal colors
  normal:
    black:   '0x282a36'
    red:     '0xff5555'
    green:   '0x50fa7b'
    yellow:  '0xf1fa8c'
    blue:    '0xbd93f9'
    magenta: '0xff79c6'
    cyan:    '0x8be9fd'
    white:   '0xbfbfbf'

  # Bright colors
  bright:
    black:   '0x555555'
    red:     '0xff6e67'
    green:   '0x5af78e'
    yellow:  '0xf4f99d'
    blue:    '0xcaa9fa'
    magenta: '0xff92d0'
    cyan:    '0x9aedfe'
    white:   '0xe6e6e6'

font:
  normal:
    family: 'FiraCode Nerd Font'
    style: Regular
  bold:
    family: 'FiraCode Nerd Font'
    style: Bold
  italic:
    family: 'FiraCode Nerd Font'
    style: Retina 

  # Point size
  size: 14.0

# Tab 缩进 
tabspaces: 4 

shell: 
	program: /bin/zsh 
	args: 
		# login 
		- -l 

# 背景透明度 
window.opacity: 0.9 
window: 
	# 窗口大小 
	dimensions: 
		columns: 120 
		lines: 60 
	# 边缘空白 
	padding: 
		x: 10 
		y: 15 
	dynamic_padding: false
	# startup_mode: Maximized 
	# 窗口修饰 full: 有边界 + 标题栏 ; none: 无边界 + 标题栏 
	decorations: none 

scrolling: 
	# 历史保留行数 
	history: 2000 
	# 每次滚动行数 
	multiplier: 20 
	# 每次滚动行数（分屏时） 
	faux_multiplier: 100 
	# 自动滚动至最新行 
	auto_scroll: true
```
