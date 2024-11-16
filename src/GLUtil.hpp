#pragma once

class TempBind {
private:
	int target;
	unsigned name;
	unsigned oldName;

public:
	TempBind(int _target, unsigned _name);
	TempBind(const TempBind& other) = delete;
	TempBind(TempBind&& other) = default;

	~TempBind();
};