#ifndef ENTITY_BASE_H
#define ENTITY_BASE_H

#include "Vector3.h"

class EntityBase
{
public:
	EntityBase();
	virtual ~EntityBase();

	virtual void Update(double _dt);
	virtual void Render();
	virtual void RenderUI();

	inline void SetPositionX(const int& _value){ position.x = _value; };
	inline Vector3 GetPositionX(){ return position.x; };

	inline void SetPositionY(const int& _value){ position.y = _value; };
	inline Vector3 GetPositionY(){ return position.y; };

	inline void SetPositionZ(const int& _value){ position.z = _value; };
	inline Vector3 GetPositionZ(){ return position.z; };

	inline void SetPosition(const Vector3& value){ position = value; };
	inline Vector3 GetPosition() { return position; };

	inline void SetScale(const int& _value){
		scale = Vector3(_value, _value, _value);
	};
	inline Vector3 GetScale(){ return scale; };

	inline void SetRotation(const float& _value) { rotation = _value };
	inline float GetRotation(){ return rotation; };

	bool IsDone();
	void SetIsDone(const bool _value);

	// Check if this entity has a collider class parent
	virtual bool HasCollider(void) const;
	// Set the flag to indicate if this entity has a collider class parent
	virtual void SetCollider(const bool _value);
	// Set the flag, bLaser
	virtual void SetIsLaser(const bool bLaser);
	// Get the flag, bLaser
	virtual bool GetIsLaser(void) const;

protected:
	Vector3 position;
	Vector3 scale;
	float rotation;



	bool isDone;
	bool m_bCollider;
	bool bLaser;
};

#endif // ENTITY_BASE_H