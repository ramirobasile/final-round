#pragma once

namespace fr {

class Dodge {
	public:
	Dodge();
	Dodge(int self_damage, float active_begin, float active_end, 
			float recovery_end);

	void start();
	void interrupt();
	void end();
	bool isStartingUp() const;
	bool isActive() const;
	bool isRecovering() const;
	bool isDone() const;

	float progress = -1;
	int self_damage;

	private:
	float active_begin;
	float active_end;
	float recovery_end;
};

} // namespace fr
