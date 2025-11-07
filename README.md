
# imgGPSinfo
读取图片中的定位信息，WGS84，转换成10进制格式。生成高德地图链接。
# 使用方法
* 点击按钮 选择单个或者多个图片文件

  <img width="376" height="234" alt="image" src="https://github.com/user-attachments/assets/f850545c-1bbc-498c-8f51-d9741932bed8" />
   
* 会弹出窗口显示 图片的地理信息 ，同时选择几个文件就会弹出对应数量的窗口（例如选择2个文件，就会弹出2个下图的窗口）  
  <img width="1340" height="809" alt="image" src="https://github.com/user-attachments/assets/72229c1f-765b-464c-81da-9c71670cecf7" />  

# 编译要求
###  需要使用 vs2026   
<img width="844" height="112" alt="image" src="https://github.com/user-attachments/assets/2ef301eb-e944-4a56-a8fd-3cd452f6b5ad" />

###  安装 MFC   
<img width="350" height="670" alt="image" src="https://github.com/user-attachments/assets/8f1b106f-80c5-4120-ac40-7fc43838f9af" />

###  安装 Exiv2
```bash
vcpkg install exiv2:x64-windows
```
### 打开文件注意
> 目前存在高版本的 visual studio 有bug，请手动打开 imginfo.vcxproj 文件或者imginfo.sln  
> ! 不要打开 imginfo.slnx
