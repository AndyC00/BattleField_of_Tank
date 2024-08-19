#ifndef   SPRITE_H 
#define   SPRITE_H 

// Forward Declarations:
class Renderer; 
class Texture;

class Sprite
{
	// Member methods:
public:
	Sprite();
	~Sprite();

	bool Initialise(Texture& texture); 
	void Process(float deltaTime); 
	void Draw(Renderer& renderer);

	int GetWidth() const;
	int GetHeight() const;

	void SetX(int x); 
	int GetX() const; 
	void SetY(int y);
	int GetY() const;

	void SetAngle(float angle); 
	float GetAngle() const;
	void SetScale(float scale); 
	float GetScale() const;

	void SetAlpha(float alpha); 
	float GetAlpha() const;

	void SetRedTint(float value); 
	float GetRedTint() const;
	void SetGreenTint(float value); 
	float GetGreenTint() const; 
	void SetBlueTint(float value); 
	float GetBlueTint() const;

protected:
	float Clamp(float minimum, float value, float maximum);

private:
	Sprite(const Sprite& sprite);
	Sprite& operator=(const Sprite& sprite);

	// Member data: 
public:

protected:
	Texture* m_pTexture; 
	int m_x;
	int m_y;
	float m_angle; 
	int m_centerX; 
	int m_centerY;

	int m_width; 
	int m_height;

	float m_scale; 
	float m_alpha;

	float m_tintRed; 
	float m_tintGreen; 
	float m_tintBlue;

private:

};

#endif //   SPRITE_H 
