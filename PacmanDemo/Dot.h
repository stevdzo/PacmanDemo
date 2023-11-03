#ifndef _DOT_H
#define _DOT_H

#include "GameObject.h"

enum DotType {

	small = 0,
	big = 1
};

class Dot : public GameObject {

private:

	int m_value;
	DotType m_type;

public:

	Dot(DotType p_type);
	~Dot();

	void setValue(int);
	int getValue(void);

	void update(float p_deltaTime) override;
	void render() override;
	void renderWireframe() override;
};
#endif