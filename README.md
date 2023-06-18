# MeshQEM-Skeleton
## 專案介紹
這是一個實作與3D模型Mesh相關的課堂專案，這個專案分成兩個部分，第一個部分是3D模型的mesh簡化，也就是減少mesh面數，我們需要實作 Surface Simplification Using Quadric Error Metrics(https://dl.acm.org/doi/10.1145/258734.258849) 這篇論文，使用QEM計算合併每個邊合併的error，error值越大代表這個邊合併後被改變的形狀越大，所以需要由error數值小的開始簡化。最後加入滑桿自由調整mesh的面數。

第二部分是實作將模型骨架化，需要實作另一篇論文 Skeleton Extraction by Mesh Contraction(https://dl.acm.org/doi/pdf/10.1145/1399504.1360643) ，論文中會透過least-square矩陣的方式把3D模型點資訊轉換到矩陣內計算，計算完後模型上的點會慢慢向normal反方向收縮，將3D模型慢慢縮成骨架的樣子。
## 使用library
在這個專案裡，我們使用C++作為主要程式語言，OpenMesh做為儲存mesh資料與mesh相關計算的資料庫，計算least-square時使用Eigen作為計算矩陣時的library，並使用Windows Form作為UI介面。

## 簡化3D模型效果
### 使用海神波賽頓模型 原始56112面

#### 整體簡化
https://github.com/YuanWeiChen16/MeshQEM-Skeleton/assets/46083885/1aefcdb7-7588-407b-a59f-732c8cfeb194
#### 上半身特寫
https://github.com/YuanWeiChen16/MeshQEM-Skeleton/assets/46083885/bef21974-78dd-49b9-9e79-10fff018c901
#### 頭部特寫
https://github.com/YuanWeiChen16/MeshQEM-Skeleton/assets/46083885/8950c0e2-7d43-4ce8-9fa9-090679050183
#### 手部特寫
https://github.com/YuanWeiChen16/MeshQEM-Skeleton/assets/46083885/bd0c68c3-1d93-40fc-ba94-6720e735a4bf
#### 基座特寫
https://github.com/YuanWeiChen16/MeshQEM-Skeleton/assets/46083885/504163e4-e7d4-4b9f-9caf-57ab681fe618
#### 三叉戟特寫
https://github.com/YuanWeiChen16/MeshQEM-Skeleton/assets/46083885/99208452-684b-4121-adca-3e8593375924
