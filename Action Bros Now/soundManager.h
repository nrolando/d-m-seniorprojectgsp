//this is th empty singleton class that nick asked for
//not sure what you are going to need so i didn't include anything


class soundManager
{
private:
	static soundManager* instance;
	soundManager();

public:
	static soundManager* getInstance();
}