# ps3

这次的作业内容比较多，但是整体还算简单

惯例，先把上一次的代码复制粘贴进去

## normal shader

main.cpp 里的`normal_fragment_shader` 已经实现好了，没有什么意义大概。

我们只需要修改rasterizer.cpp 里的 `rasterize_triangle` 实现即可

这个函数和上次的实现区别不大，只需要实现几个插值。插值的重心坐标也算出来了，所以只需要理解含义……

颜色、纹理、法线不需要说，`interpolated_shadingcoords` 的含义比较迷惑，它其实表示的光照发生的物体表面的点，插值来源是view_pos，这个数组的值是三角形的顶点在相机空间的坐标（投影变换前）。

然后就可以命令行运行，vs直接运行的需要注意main函数 `active_shader` 的修改

![Untitled](ps3%200058c8386d144fda83bfe37a6760433b/Untitled.png)

## Blinn-Phong

实现公式即可

需要注意：

1. norm，范式，可以理解为向量的模长
2. 光照计算时的各个向量，除了系数都是单位向量，需要normalized

```cpp
Eigen::Vector3f ambientLight(const Eigen::Vector3f& Ia, const Eigen::Vector3f& ka)
{
    return ka.cwiseProduct(Ia);
}

Eigen::Vector3f diffuseLight(const Eigen::Vector3f& kd, const light& l, const Vector3f& pos, const Vector3f& normal)
{
    const float dis = (l.position - pos).squaredNorm();
    return kd.cwiseProduct(l.intensity) / dis * std::max(0.0f, normal.dot((l.position - pos).normalized()));
}

Eigen::Vector3f specularLight(const Eigen::Vector3f& ks, const light& l, const Vector3f& pos, const Vector3f& normal, const Vector3f& eye_pos, const double p)
{
    const float dis = (l.position - pos).squaredNorm();
    const Vector3f h = ((eye_pos - pos).normalized() + (l.position - pos).normalized()).normalized();
    return ks.cwiseProduct(l.intensity) / dis * (std::powf(std::max(0.0f, normal.dot(h)), p));
}
```

![Untitled](ps3%200058c8386d144fda83bfe37a6760433b/Untitled%201.png)

## 纹理

纹理和phong模型的区别只有一行，漫反射的系数从纹理中获取

```cpp
return_color = payload.texture->getColor(payload.tex_coords.x(), payload.tex_coords.y());
```

需要注意，如果是在vs中直接运行的话，要把判断函数里的`texture_path` 移到外面

![Untitled](ps3%200058c8386d144fda83bfe37a6760433b/Untitled%202.png)

## Bump Mapping

首先需要理解法线贴图

贴图里面存的是什么？答案是切线空间里的向量，用来表示新的法线。如果法线不变的话就是（0,0,1），所以会看到法线贴图整体偏蓝。

如何算出切线空间

计算机图形学八：纹理映射的应用(法线贴图，凹凸贴图与阴影贴图等相关应用的原理详解) - 孙小磊的文章 - 知乎
https://zhuanlan.zhihu.com/p/144357517

我们得到了切线空间的矩阵TBN，然后用TBN乘上贴图里的发现向量就得到了新的法线（相机坐标系下）

bump存的是高度信息，而不是法线贴图。见上文，如何从高度推出法线

![Untitled](ps3%200058c8386d144fda83bfe37a6760433b/Untitled%203.png)

## Displacement Mapping

和bump区别是，贴图里存的也是高度，但是得到新的法线之后需要参与光照计算，

![Untitled](ps3%200058c8386d144fda83bfe37a6760433b/Untitled%204.png)