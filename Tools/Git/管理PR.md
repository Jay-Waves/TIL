
## 创建PR

### 基于当前分支的修改创建分支

`git stash`

`git checkout -b <new-heature>`

`git stash pop` 应用stash到当前分支, 并删除stash区. 如果不想删除, 用`git stash apply`. stash 用于保存不想提交的修改, add(stage)用于保存准备提交的修改.

### 基于新分支创建 pull request

fork别人仓库后, 创建一个新分支. 

推送修改: `git push origin <new_local_branch>[:<remote-branch-name>]`

上github, 建立一个pull request即可.

如果向合并入主分支, 需要先确保和origin/main同步: `git pull origin main`

## 从其他分支获取文件

从 main 分支获取 target_file 到当前分支暂存区.
```shell
git checkout main target_file
```

提交暂存区, 该文件就会被合并到分支:
```shell
git commit -m "checkout target_file from main"
```

如果在错误分支提交, 可以使用 `git reset --hard ORIG_HEAD` 来取消 merge 操作的改动.

### 合并其他分支

`git fetch origin` 获取远程改动, 不合并

`git merge origin/main` 将远程主分支和当前分支合并

如果已经在 main 上合并了远程分支, 那么可以 `git merge main` 即可.

如果不想直接创建一个新提交, 就用 `git merge --squash`

***

## 合并 PR

仓库接受PR有两种方式:
- Merge Commit: 合并时会创建一个新提交, 每个PR都有更清晰的合并点.
- Fast-Forward: 无冲突时, git会将主分支移动到PR分支同位置, *不创建新的合并提交.*

Github上可以选择仓库接受PR的方式:
- Create a Merge Commit
- Squash and Merge: 将PR分支的所有更改压缩为一个新的提交然后合并到主分支. 清除PR分支的历史, 仅保留一个干净的提交.
- Rebase and Merge: 将PR分支的更改及历史变基到主分支 (无新提交), 历史会变成*线性*