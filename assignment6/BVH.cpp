#include <algorithm>
#include <cassert>
#include "BVH.hpp"

BVHAccel::BVHAccel(std::vector<Object*> p, int maxPrimsInNode,
    SplitMethod splitMethod)
    : maxPrimsInNode(std::min(255, maxPrimsInNode)), splitMethod(splitMethod),
    primitives(std::move(p))
{
    time_t start, stop;
    time(&start);
    if (primitives.empty())
        return;
    if (splitMethod == SplitMethod::NAIVE)
        root = recursiveBuild(primitives);
    else
        root = recursiveBuildSAH(primitives);

    time(&stop);
    double diff = difftime(stop, start);
    int hrs = (int)diff / 3600;
    int mins = ((int)diff / 60) - (hrs * 60);
    int secs = (int)diff - (hrs * 3600) - (mins * 60);

    printf(
        "\rBVH Generation complete: \nTime Taken: %i hrs, %i mins, %i secs\n\n",
        hrs, mins, secs);
}

BVHBuildNode* BVHAccel::recursiveBuild(std::vector<Object*> objects)
{
    BVHBuildNode* node = new BVHBuildNode();

    // Compute bounds of all primitives in BVH node
    Bounds3 bounds;
    for (int i = 0; i < objects.size(); ++i)
        bounds = Union(bounds, objects[i]->getBounds());
    if (objects.size() == 1) {
        // Create leaf _BVHBuildNode_
        node->bounds = objects[0]->getBounds();
        node->object = objects[0];
        node->left = nullptr;
        node->right = nullptr;
        return node;
    }
    else if (objects.size() == 2) {
        node->left = recursiveBuild(std::vector{ objects[0] });
        node->right = recursiveBuild(std::vector{ objects[1] });

        node->bounds = Union(node->left->bounds, node->right->bounds);
        return node;
    }
    else {
        Bounds3 centroidBounds;
        for (int i = 0; i < objects.size(); ++i)
            centroidBounds =
            Union(centroidBounds, objects[i]->getBounds().Centroid());
        int dim = centroidBounds.maxExtent();
        switch (dim) {
        case 0:
            std::sort(objects.begin(), objects.end(), [](auto f1, auto f2) {
                return f1->getBounds().Centroid().x <
                    f2->getBounds().Centroid().x;
                });
            break;
        case 1:
            std::sort(objects.begin(), objects.end(), [](auto f1, auto f2) {
                return f1->getBounds().Centroid().y <
                    f2->getBounds().Centroid().y;
                });
            break;
        case 2:
            std::sort(objects.begin(), objects.end(), [](auto f1, auto f2) {
                return f1->getBounds().Centroid().z <
                    f2->getBounds().Centroid().z;
                });
            break;
        }

        auto beginning = objects.begin();
        auto middling = objects.begin() + (objects.size() / 2);
        auto ending = objects.end();

        auto leftshapes = std::vector<Object*>(beginning, middling);
        auto rightshapes = std::vector<Object*>(middling, ending);

        assert(objects.size() == (leftshapes.size() + rightshapes.size()));

        node->left = recursiveBuild(leftshapes);
        node->right = recursiveBuild(rightshapes);

        node->bounds = Union(node->left->bounds, node->right->bounds);
    }

    return node;
}

Intersection BVHAccel::Intersect(const Ray& ray) const
{
    Intersection isect;
    if (!root)
        return isect;
    isect = BVHAccel::getIntersection(root, ray);
    return isect;
}

Intersection BVHAccel::getIntersection(BVHBuildNode* node, const Ray& ray) const
{
    // TODO Traverse the BVH to find intersection
    Intersection res;
    const auto dirIsNeg = std::array<int, 3> {
        static_cast<int>(ray.direction.x > 0),
            static_cast<int>(ray.direction.y > 0),
            static_cast<int>(ray.direction.z > 0),
    };
    if (node == nullptr || !node->bounds.IntersectP(ray, ray.direction_inv, dirIsNeg))
    {
        return res;
    }
    if (node->left == nullptr && node->right == nullptr)
    {
        if (node->object != nullptr)
            res = node->object->getIntersection(ray);
        return res;
    }
    const auto l = getIntersection(node->left, ray);
    const auto r = getIntersection(node->right, ray);
    if (l.happened && l.distance < res.distance)
    {
        res = l;
    }
    if (r.happened && r.distance < res.distance)
    {
        return r;
    }
    return res;
}

BVHBuildNode* BVHAccel::recursiveBuildSAH(std::vector<Object*> objects)
{
    BVHBuildNode* node = new BVHBuildNode();

    Bounds3 bounds;
    for (int i = 0; i < objects.size(); ++i)
        bounds = Union(bounds, objects[i]->getBounds());
    if (objects.size() == 1) {
        node->bounds = objects[0]->getBounds();
        node->object = objects[0];
        node->left = nullptr;
        node->right = nullptr;
        return node;
    }
    if (objects.size() == 2) {
        node->left = recursiveBuildSAH(std::vector{ objects[0] });
        node->right = recursiveBuildSAH(std::vector{ objects[1] });
        node->bounds = Union(node->left->bounds, node->right->bounds);
        return node;
    }

    const float sumArea = bounds.SurfaceArea();
    Bounds3 centroidBounds;
    for (int i = 0; i < objects.size(); ++i)
        centroidBounds =
        Union(centroidBounds, objects[i]->getBounds().Centroid());
    int dim = centroidBounds.maxExtent();
    Vector3f delta, dig = centroidBounds.Diagonal();
    if (dim == 0)
        delta.x = dig.x / static_cast<double>(buckets_);
    else if (dim == 1)
        delta.y = dig.y / static_cast<double>(buckets_);
    else if (dim == 2)
        delta.z = dig.z / static_cast<double>(buckets_);
    std::vector<int> primitivesNum(buckets_, 0);
    std::vector<Bounds3> bounds3s(buckets_);

    Vector3f l = centroidBounds.pMin, r = centroidBounds.pMax - (buckets_ - 1) * delta;
    for (int i = 0; i < buckets_; ++i, l += delta, r += delta)
    {
        auto ithBound = Bounds3(l, r);
        for (const auto& obj : objects)
        {
            auto b = obj->getBounds();
            Vector3f center = b.Centroid();
            if (ithBound.Inside(center, ithBound))
            {
                primitivesNum[i] += 1;
                bounds3s[i] = Union(bounds3s[i], b);
            }
        }
    }

    std::vector<float> sa(buckets_ - 1, 0);
    Bounds3 tmpBound;
    for (int i = 0; i < buckets_ - 1; ++i)
    {
        tmpBound = Union(tmpBound, bounds3s[i]);
        sa[i] += primitivesNum[i] * tmpBound.SurfaceArea();
    }
    tmpBound = Bounds3();
    float mins = std::numeric_limits<float>::max(); int mi = 1;
    for (int i = buckets_ - 1; i > 0; --i)
    {
        tmpBound = Union(tmpBound, bounds3s[i]);
        sa[i - 1] += primitivesNum[i] * tmpBound.SurfaceArea();
        if (sa[i - 1] < mins)
        {
            mins = sa[i - 1];
            mi = i;
        }
    }
    std::vector<Object*> leftshapes, rightshapes;
    auto lbound = Bounds3(centroidBounds.pMin, centroidBounds.pMax - (buckets_ - mi) * delta);
    for (const auto& obj : objects)
    {
        auto b = obj->getBounds();
        Vector3f center = b.Centroid();
        if (lbound.Inside(center, lbound))
        {
            leftshapes.push_back(obj);
        }
        else
        {
            rightshapes.push_back(obj);
        }
    }
    node->left = recursiveBuildSAH(leftshapes);
    node->right = recursiveBuildSAH(rightshapes);
    node->bounds = Union(node->left->bounds, node->right->bounds);
    return node;
}