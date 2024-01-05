#ifndef _DROP_H
#define _DROP_H

#include "Entity.h"

class Drop : public Entity {

private:

	int m_value;

public:

	Drop(Sprite p_sprite);

	void update(float p_deltaTime) override;
	void render() override;
	void renderWireframe() override;

	~Drop();
};
#endif