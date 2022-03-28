# zxx-RM-A-Template

我的RM-A开发板模板。



### 第一次推送流程：

1. 在Github中添加生成的ssh密钥

2. 在本地项目文件中目录下：

   ```
   # 测试连接
   ssh -T git@github.com
   
   # 复制仓库ssh地址，替换下面git@...部分
   git remote add origin git@github.com:xxx/xxx.git
   
   # 初始化本地库 -> 添加要提交的文件 -> 编写commit说明 -> 查看状态
   git init 
   git add . 
   git commit -m “What have you done.”
   git status 
   ```

   

 3. 推送代码

    ```
    # 注意分支是否为`master`
    git push -u origin master
    ```

    

