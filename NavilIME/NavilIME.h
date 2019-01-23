#pragma once

struct ImeMenu {
	INT id;
	LPCWSTR text;
};

class NavilIME
{
public:
	NavilIME();
	~NavilIME();

	bool GetHangulMode();
	bool SetHangulMode(bool mode);
	void ToggleHangulMode();
	
	void HangulNew();
	bool HangulProcess(int ascii);
	bool HangulBackspace();
	UINT HangulGetPreedit(UINT idx);
	UINT HangulGetCommit(UINT idx);
	UINT HangulFlush();

private:
	bool _IsHangulModeOn;
};