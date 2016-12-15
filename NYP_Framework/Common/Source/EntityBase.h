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
	inline float GetPositionX(){ return position.x; };

	inline void SetPositionY(const int& _value){ position.y = _value; };
	inline float GetPositionY(){ return position.y; };

	inline void SetPositionZ(const int& _value){ position.z = _value; };
	inline float GetPositionZ(){ return position.z; };

	inline void SetPosition(const Vector3& value){ position = value; };
	inline Vector3 GetPosition() { return position; };

	inline void SetScaleX(const int& _value){
		scale.x = _value;
	};
	inline float GetScaleX(){ return scale.x; };
	inline void SetScaleY(const int& _value){
		scale.y = _value;
	};
	inline float GetScaleY(){ return scale.y; };
	inline void SetScaleZ(const int& _value){
		scale.z = _value;
	};
	inline float GetScaleZ(){ return scale.z; };

	inline void SetScale(const Vector3& _value){ scale = _value; };
	inline Vector3 GetScale(){ return scale; };

	inline void SetRotation(const float& _value) { rotation = _value; };
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

	virtual void SetIsZombie(const bool isZombie);
	virtual bool GetIsZombie(void) const;

	virtual void SetIsProjectile(const bool isProjectile);
	virtual bool GetIsProjectile(void) const;

protected:
	Vector3 position;
	Vector3 scale;
	float rotation;



	bool isDone;
	bool m_bCollider;
	bool bLaser;
	bool isZombie;
	bool isProjectile;
};

#endif // ENTITY_BASE_H