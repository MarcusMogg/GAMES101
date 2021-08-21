非常简单一个递归

```cpp
cv::Point2f recursive_bezier(const std::vector<cv::Point2f>& control_points, float t)
{
    if (control_points.size() == 1) return control_points[0];
    std::vector<cv::Point2f> nps;
    for (int i = 1; i < control_points.size(); ++i)
    {
        nps.emplace_back(t * control_points[i - 1] + (1 - t) * control_points[i]);
    }
    return recursive_bezier(nps, t);
}
```

`bezier` 函数和`naive_bezier` 过程一模一样，除了把颜色设置为绿色（没看文档，盲猜第二个是Green）。
