## 外观

```json
{
	/*appearance*/
	"editor.fontFamily": "Fira Code Retina", //连字字体
	"editor.fontLigatures": true,
	"debug.console.fontFamily": "Consolas",
	"editor.unicodeHighlight.ambiguousCharacters": false, //区分易混淆标点，它会高亮中文标点
	"editor.inlayHints.fontSize": 10,
	"editor.suggestFontSize": 12,
	"debug.console.fontSize": 12,
	"editor.fontSize": 16,
	"editor.semanticHighlighting.enabled": true, // 语法高亮一般是主题提供的
	"window.zoomLevel": -1,
	"window.menuBarVisibility": "compact", //环境
	
	// theme
	"workbench.colorTheme": "GitHub Dark Dimmed",
	//"window.autoDetectColorScheme": true, //根据电脑主题，切换黑夜白天
	//"workbench.preferredDarkColorTheme": "GitHub Dark Colorblind (Beta)",
	//"workbench.preferredLightColorTheme": "Noctis Hibernus",
	"workbench.iconTheme": "material-icon-theme",
	"material-icon-theme.folders.associations": {},
}
```

## 编辑器

```json
{
	// line scroll & wrap & tab
	"editor.lineNumbers": "interval", //行号显示
	"editor.wrappingStrategy": "advanced",
	"editor.wordWrap": "on",
	"editor.mouseWheelScrollSensitivity": 4,
	"editor.smoothScrolling": true,
	"editor.scrollbar.verticalScrollbarSize": 10,
	"editor.scrollbar.horizontalScrollbarSize": 5,
	"editor.detectIndentation": false,
	"workbench.editor.scrollToSwitchTabs": true,
	"workbench.editor.wrapTabs": true,
	"rewrap.wrappingColumn": 80,
	"editor.tabSize": 2,

	/*代码提示, Code Lint*/
	"editor.tabCompletion": "on", //use tab to select hint
	"editor.quickSuggestions": {
		"other": "on",
		"comments": "off",
		"strings": "on"
	},
	"editor.suggest.showInlineDetails": false,
	"editor.suggest.showTypeParameters": false,
	"editor.inlayHints.enabled": "offUnlessPressed",

	/* 代码格式化, Code Format */
	"editor.formatOnPaste": true,
	"editor.formatOnType": true,
	"editor.renderWhitespace": "none",
	"editor.minimap.enabled": false,
	"editor.guides.bracketPairs": "active",

}
```

## 语言

```json
{
	// markdown
	"[markdown]": {
		"editor.defaultFormatter": "yzhang.markdown-all-in-one",
		"editor.wordWrap": "on"
	},
	/*extension: markdown preview*/
	"markdown-preview-enhanced.breakOnSingleNewLine": false,
	"markdown-preview-enhanced.codeBlockTheme": "vue.css",
	"markdown-preview-enhanced.imageFolderPath": "/附件",
	"markdown-preview-enhanced.previewTheme": "gothic.css",
	"markdown.preview.fontSize": 16,
	"notebook.outputLineHeight": 18,
	"markdown-preview-enhanced.enableEmojiSyntax": false,


	/*extension: wolfram language notebook*/
	"wolframLanguageNotebook.kernel.configurations": {
		//这玩意只识别全局设置文件。。
		"wolframscript": {
			"type": "local",
			"command": "wolframscript",
			"ports": "49152-65535"
		}
	},
	// python
	"[python]": {
		"editor.formatOnType": true,
		"python.analysis.inlayHints.callArgumentNames": true,
		"python.analysis.inlayHints.variableTypes": true,
		"python.analysis.autoImportCompletions": true,
		"python.pythonPath": "F:\\python\\Python310\\python.exe",
	},

	// c++
	// debugger
	"lldb.suppressUpdateNotifications": true,
	"lldb.launch.terminal": "integrated",
	"lldb.library": "F:\\c\\clang\\LLVM\\bin",
	"C_Cpp.inlayHints.autoDeclarationTypes.enabled": true,
	"C_Cpp.inlayHints.autoDeclarationTypes.showOnLeft": true,
	"C_Cpp.default.cppStandard": "c++20",
	"C_Cpp.intelliSenseEngineFallback": "enabled",
	"[cpp]": {
		"editor.defaultFormatter": "ms-vscode.cpptools",
	},
	// formatter 
	"C_Cpp.formatting": "vcFormat",
	"C_Cpp.vcFormat.newLine.beforeElse": false,
	"C_Cpp.vcFormat.newLine.beforeCatch": false,
	"C_Cpp.vcFormat.newLine.beforeOpenBrace.function": "sameLine",
	"C_Cpp.vcFormat.newLine.beforeOpenBrace.block": "sameLine",
	"C_Cpp.vcFormat.newLine.beforeOpenBrace.lambda": "sameLine",
	"C_Cpp.vcFormat.newLine.beforeOpenBrace.namespace": "sameLine",
	"C_Cpp.vcFormat.newLine.beforeOpenBrace.type": "sameLine",
	// clang tidy static analysis
	"C_Cpp.codeAnalysis.clangTidy.enabled": true,
	"C_Cpp.codeAnalysis.clangTidy.checks.disabled": [
	    "modernize-use-trailing-return-type",
	    "modernize-use-nodiscard",
	    "readability-braces-around-statements",
	    "readability-magic-numbers",
	    "performance-avoid-endl",
	    "cppcoreguidelines-avoid-magic-numbers"
	  ],
	"C_Cpp.codeAnalysis.clangTidy.checks.enabled": [
	    "modernize-*",
	    "bugprone-*",
	    "performance-*",
	    "readability-*"
	  ],
	// compiler
	"cmake.showOptionsMovedNotification": false,
	
	// c
	"[c]": {
	},

	// go
	"go.inlayHints.constantValues": true,
	"go.inlayHints.parameterNames": true,
	"[go]": {
		"editor.codeActionsOnSave": {
			"source.organizeImports": "never"
		}
	},

	//latex
	"latex-workshop.synctex.synctexjs.enabled": true,
	"latex-workshop.synctex.afterBuild.enabled": true,
	"latex-workshop.view.pdf.viewer": "tab",
	// "latex-workshop.latex.autoBuild.run": "never",
	// "latex-workshop.latex.clean.fileTypes": [
	//   "%DOCFILE%.aux",
	//   "%DOCFILE%.bbl",
	//   "%DOCFILE%.blg",
	...
	// ],
	// "latex-workshop.latex.autoClean.run": "onSucceeded",
	"[latex]": {
		"editor.defaultFormatter": "James-Yu.latex-workshop"
	},
	"latex-workshop.hover.preview.cursor.color": "magenta",

	// 其他鱼杂语言
	"[vue]": {
	 	"editor.defaultFormatter": "Vue.volar"
	},
	"[typescript]": {
	 	"editor.defaultFormatter": "vscode.typescript-language-features"
	},
}
```

## 远程

```json
	// remote 
	"remote.SSH.remotePlatform": {
		"10.130.147.18": "linux",
		"192.168.1.107": "linux",
		"rozz-server-inlan": "linux",
		"rozz-server-outlan": "linux"
	},
  "remote.autoForwardPortsSource": "hybrid",

```

## 其他

```json
{
	"security.workspace.trust.untrustedFiles": "open",

	//集成终端
	"terminal.integrated.enableMultiLinePasteWarning": false,
	"terminal.integrated.defaultProfile.windows": "Command Prompt",
	"terminal.integrated.defaultProfile.linux": "zsh",
	"terminal.integrated.fontSize": 13,
	"terminal.integrated.copyOnSelection": true,

	/* 资源管理器设置, explorer behavior */
	"explorer.autoReveal": false,
	"explorer.enableUndo": true, //资源管理器设置
	"explorer.confirmDelete": false,
	"explorer.confirmDragAndDrop": false,
	"editor.largeFileOptimizations": false,

	//project manager
	"projectManager.tags": [
		"Personal",
		"Work",
		"Tools",
		"Notes",
		"Project",
		"Env"
	],
	"projectManager.sortList": "Path",

	// 文件类型关联
	"workbench.editorAssociations": {
		"*.nb": "default",
		"*.txt": "default",
		"*.db": "sqlite-viewer.option",
		//"*.pdf": "pdf.preview"
	},

	//git
	"git.enabled": false,
	"git.confirmSync": false,
	"git.enableSmartCommit": true,
	"git.autofetch": false,
	"git.openRepositoryInParentFolders": "never",

	//不同步的插件：
	"settingsSync.ignoredExtensions": [
		"buuug7.chinese-punctuation-to-english",
		"ms-python.vscode-pylance",
		"mutable-ai.mutable-ai",
		"jaybarnes.chatgpt-vscode-plugin",
		"mshr-h.veriloghdl"
	],

	//extension: AI
	"aws.suppressPrompts": {
		"codeWhispererConnectionExpired": true,
		"codeWhispererNewWelcomeMessage": true
	},
}
```

## vim

```json
{
	/*extension: vim */
	"vim.easymotion": true,
	"vim.incsearch": true,
	"vim.useSystemClipboard": false,
	"vim.useCtrlKeys": true,
	"vim.hlsearch": false, //结果一直高亮
	"vim.foldfix": true,
	// "vim.insertModeKeyBindings": [
	//   {
	//     "before": [
	//       "j",
	//       "k"
	//     ],
	//     "after": [
	//       "<Esc>"
	//     ]
	//   }
	// ],
	//映射空格键为 easymotion Start of word 指令快捷键
	"vim.normalModeKeyBindingsNonRecursive": [
		{
			"before": [ //向下翻页改成了c-n， 因为实在改不过来c-f的查找绑定键
				"<C-n>"
			],
			"after": [
				"<C-f>"
			]
		},
		// easymotion 快速跳转 ,s + <char>
		{
			"before": [
				"<leader>",
				"s"
			],
			"after": [
				"<leader>",
				"<leader>",
				"s"
			]
		},
	],
	"vim.leader": ",",
	"vim.handleKeys": {
		"<C-a>": false,
		"<C-f>": false
	},
	// 在中英文输入法间自动切换
	"vim.autoSwitchInputMethod.enable": true,
	"vim.autoSwitchInputMethod.obtainIMCmd": "E:\\tools\\text\\ObsidianPlugin\\im-select.exe",
	"vim.autoSwitchInputMethod.switchIMCmd": "E:\\tools\\text\\ObsidianPlugin\\im-select.exe {im}",
	"vim.autoSwitchInputMethod.defaultIM": "English",
	"vim.cursorStylePerMode.replace": "underline",
}
```