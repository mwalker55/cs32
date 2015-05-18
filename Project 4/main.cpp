#include "provided.h"
#include "http.h"
#include <iostream>
using namespace std;

int main()
{
	string pageText =
		"<html>\n"
		"<head>\n"
		" <title>My Pretend Web Page</title>\n"
		"</head>\n"
		"<body>\n"
		"<h2>My Pretend Web Page<h1>\n"
		"<p>\n"
		"I wish I were creative enough to have something interesting \n"
		"to say here.  I'm not, though, so this is a boring page.\n"
		"</p>\n"
		"<p>\n"
		"Oh, well.\n"
		"</p>\n"
		"</body>\n"
		"</html>\n"
		;
	HTTP().set("http://boring.com", pageText);
	string plan;
	for (int i = 0; i < 200000; i++)
		plan.push_back(static_cast<char>(rand() % 256));
	if (!WebSteg::hideMessageInPage("http://boring.com", plan, pageText))
	{
		cout << "Error hiding!" << endl;
		return 1;
	}
	HTTP().set("http://boring.com", pageText);
	string msg;
	if (!WebSteg::revealMessageInPage("http://boring.com", msg))
	{
		cout << "Error revealing!" << endl;
		return 1;
	}
	if (msg != plan)
	{
		cout << "Message not recovered properly:\n" << msg;
		return 1;
	}
	cout << "Recovered text is what was hidden:\n" << msg << endl;
}
