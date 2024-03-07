Stable Diffusion 深度学习模型有几个关键部分:
- Checkpoints model: 如 pruned-sd1.5, sd2.1, sdxl. 别人将训练好的模型状态保存下来, 供别人加载和复现 (类似 docker 一层镜像).
- VAE model (Variational Autoencoder, 变分自编码器): 找一个凑合用就行, 底座模型内一般内置了基础 vae, 用于学习输入数据的潜在表示, 压缩图像维度, 有助于减少计算负担和提高图像质量.
- LoRA model (Low-Rank Adaptation, 低秩模型): 用于调整底座模型(base model checkpoints)参数的方法, 在底座基础上进行参数微调, 使其适应某一类图像主题.
- ~~Refiner: 对生成图像进一步增强, 优化细节, 修复噪声?~~

挑选 base model 时, 可以参考: 
1. 上传时间和版本, 一般越新效果越好 (当然不排除越训练越回去的)
2. 看其 base model 模型版本, 基于 SD1.5 的对显存和显卡性能比较友好 (<8G), SD2.0 明显出图变慢,  SDXL 3070 基本跑不动 (>12GB). SD1.5 的模型体积都比较小 (2GB左右), 但是 SDXL 就 6GB 以上了. 
3. SD1.5 和 SD2.0 图像都是 512*512 清晰度, SDXL 默认则是 1024*1024. SDXL 对文字/人体结构细节生成更加准确. 同时 SDXL 不需要太多 prompt 咒语 (施法前摇), 对自然语言的提示词更加友好. SDXL 对多风格支持也更好, 不太需要一个主题换一个 checkpoints+lora.

参数:
- prompts: 
- negative prompts: 告诉模型什么标签不能画, 比如 `deformed limb`
- clipskip:
- sampler: 采样器
- steps: 迭代次数
