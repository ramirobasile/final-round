#pragma once

namespace fr {

class Health {
	public:
	Health() {};
	Health(int max, int min, int health_regen, float regen_rate);
	
	void regen(float dt);
	void takeDamage(int damage);
	void takePermaDamage(int damage);
	void resetRegen();
	int getMin() const;
	int getMax() const;
	int getCurrent() const;
	
	private:
	int max = -1;
	int min = -1;
	int health_regen = -1;
	float regen_rate = -1;
	int current = -1;
	float tt_regen = 0;
};

} // namespace fr
