#ifndef __SCENE_H_
#define __SCENE_H_

//Forward declarations:
class Renderer;
class InputSystem;

//class declaration:
class Scene
{
	//member methods:
public:
	Scene();
	virtual ~Scene();

	virtual bool Initialise(Renderer& renderer) = 0;
	virtual void Process(float deltaTime, InputSystem& inputSystem) = 0;
	virtual void Draw(Renderer& renderer) = 0;

protected:

private:
	Scene(const Scene& scene);
	Scene& operator = (const Scene& scene);

	//member data:
public:
	virtual void DebugDraw() = 0;

protected:

};

#endif __SCENE_H_