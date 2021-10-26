#include "Scene.hpp"
#include "Sphere.hpp"
#include "Triangle.hpp"
#include "Light.hpp"
#include "Renderer.hpp"

// In the main function of the program, we create the scene (create objects and lights)
// as well as set the options for the render (image width and height, maximum recursion
// depth, field-of-view, etc.). We then call the render function().
int main()
{

    //抓到作业先看main 说明有写。
    //流程大致是

    //屏幕分辨率设置 and 场景对象创建；
    Scene scene(1280, 960);
    //智能指针指向一个球类；
    auto sph1 = std::make_unique<Sphere>(Vector3f(-1, 0, -12), 2);
    //球类有个父类叫物体。 里面有materialType 材质类 材质类只有三种值： 漫反射和自身发光，折射和反射， 反射
    //材质定义；
    sph1->materialType = DIFFUSE_AND_GLOSSY;
    //漫反射颜色；
    sph1->diffuseColor = Vector3f(0.6, 0.7, 0.8);

    //同理；
    auto sph2 = std::make_unique<Sphere>(Vector3f(0.5, -0.5, -8), 1.5);
    //折射率
    sph2->ior = 1.5;
    //reflection  refraction
    sph2->materialType = REFLECTION_AND_REFRACTION;
    //加入物体到场景；
    scene.Add(std::move(sph1));
    scene.Add(std::move(sph2));
    //创建值；
    //uint32_t 类型无符号32位 int是64位；  很少用无符号忘了。。
    Vector3f verts[4] = {{-5, -3, -6}, {5, -3, -6}, {5, -3, -16}, {-5, -3, -16}};
    uint32_t vertIndex[6] = {0, 1, 3, 1, 2, 3};
    Vector2f st[4] = {{0, 0}, {1, 0}, {1, 1}, {0, 1}};

    //MeshTriangle 类型网格三角形， 父类 对象
    auto mesh = std::make_unique<MeshTriangle>(verts, vertIndex, 2, st);
    //材质 漫反射和自身发光。
    mesh->materialType = DIFFUSE_AND_GLOSSY;

    //加入对象到场景；
    //加入光到场景；
    scene.Add(std::move(mesh));
    scene.Add(std::make_unique<Light>(Vector3f(-20, 70, 20), 0.5));
    scene.Add(std::make_unique<Light>(Vector3f(30, 50, -12), 0.5));

    //最大问题:
    Renderer r;
    r.Render(scene); //这行到底干了啥？
    return 0;
}