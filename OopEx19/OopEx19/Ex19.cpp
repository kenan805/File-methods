#include<iostream>
#include<string>
#include<windows.h>
#include<ctime>
using namespace std;

class Disc {
private:
	class File {
	private:
		double _file_size = 0;
		string _filename = "";
		string _content = "";
		string _createdTime = "";
		string _changedTime = "";
	public:
		File(string filename, string content = "", string createdTime = __TIME__, string changedTime = __TIME__) {
			SetFilename(filename);
			SetFileSize(content.size());
			SetContent(content);
			SetCreatedTime(createdTime);
			SetChangedTime(changedTime);
		}

		double GetFileSize()const
		{
			return _file_size;
		}
		string GetFilename()const
		{
			return _filename;
		}
		string GetContent()const
		{
			return _content;
		}
		string GetCreatedTime()const
		{
			return _createdTime;
		}
		string GetChangedTime()const
		{
			return _changedTime;
		}

		void SetFileSize(const double file_size)
		{
			try
			{
				if (file_size < 0) throw "error";
			}
			catch (const char* ex)
			{
				cout << "The file size cannot be negative !" << endl;
			}

			_file_size = file_size;
		}
		void SetFilename(const string& filename) {
			if (filename.empty()) return;
			_filename = filename;
		}
		void SetContent(const string& content) {
			if (content.empty()) return;
			_content = content;
		}
		void SetCreatedTime(const string& createdTime) {
			if (createdTime.empty()) return;

			time_t currently = time(0);
			char dt[30]{};
			ctime_s(dt, sizeof dt, &currently);
			string time = dt;

			_createdTime = dt;
		}
		void SetChangedTime(const string& changedTime) {
			if (changedTime.empty()) return;

			time_t currently = time(0);
			char dt[30]{};
			ctime_s(dt, sizeof dt, &currently);
			string time = dt;

			_changedTime = dt;
		}


	};



	File** files = NULL;
	int file_count = 0;
	double current_size = 0;
	string _discName = "";
public:
	Disc(const string& name)
	{
		this->_discName = name;
	}

	const static double max_size;

	File& getFile(string filename) {
		if (!isExistsFile(filename)) {
			ShowAllFiles();
			throw "File not found";
		}

		for (size_t i = 0; i < file_count; i++)
		{
			if (files[i]->GetFilename() == filename) return *files[i];
		}
	}

	bool isExistsFile(string filename) {
		for (size_t i = 0; i < file_count; i++)
		{
			if (filename == files[i]->GetFilename()) return true;
		}
		return false;
	}

	virtual void createFile(string filename, string content = "", string createdTime = __TIME__) {
		if (isExistsFile(filename)) {
			ShowAllFiles();
			throw "File already exist";
		}
		auto newFiles = new File * [file_count + 1];

		for (size_t i = 0; i < file_count; i++)
		{
			newFiles[i] = files[i];
		}

		File* file = new File(filename, content, createdTime);

		newFiles[file_count] = file;

		delete[] files;
		files = newFiles;
		newFiles = nullptr;

		current_size += content.length();
		file_count++;


		if (current_size > max_size) throw "The disc is full";

	}

	virtual void setFileContent(string filename, string content = "", string changedTime = __TIME__) {
		if (!isExistsFile(filename)) {
			ShowAllFiles();
			throw "File not found";
		}
		for (size_t i = 0; i < file_count; i++)
		{
			if (files[i]->GetFilename() == filename) {
				current_size -= files[i]->GetContent().size();
				files[i]->SetFileSize(content.size());


				files[i]->SetContent(content);
				current_size += content.size();
				if (current_size > max_size) throw "The disc is full";
			}
		}
	}

	void deleteFileContent(string filename) {
		if (!isExistsFile(filename)) {
			ShowAllFiles();
			throw "File not found";
		}
		for (size_t i = 0; i < file_count; i++)
		{
			if (files[i]->GetFilename() == filename) {
				current_size -= files[i]->GetContent().size();
				files[i]->SetContent(" ");
				files[i]->SetFileSize(0);
			}
		}
	}

	void deleteFileByFilename(string filename) {
		if (!isExistsFile(filename)) {
			ShowAllFiles();
			throw "File not found";
		}

		auto newFiles = new File * [file_count - 1];
		int index;
		for (size_t i = 0; i < file_count; i++)
		{
			if (files[i]->GetFilename() == filename) {
				current_size -= files[i]->GetContent().size();
				index = i;
				break;
			}
		}
		for (size_t j = 0, i = 0; j < index; j++, i++)
		{
			newFiles[j] = files[i];
		}
		for (size_t j = index; j < file_count; j++, index++)
		{
			newFiles[j] = files[index + 1];
		}


		file_count--;

		delete[] files;
		files = newFiles;
		newFiles = nullptr;

	}

	void updateFilename(string oldFilename, string newFilename) {
		if (!isExistsFile(oldFilename)) {
			ShowAllFiles();
			throw "File not found";
		}

		for (size_t i = 0; i < file_count; i++)
		{
			if (files[i]->GetFilename() == oldFilename) {
				files[i]->SetFilename(newFilename);
			}
		}
	}

	int HashCode(const string& str) {
		int h = 0;
		for (size_t i = 0; i < str.size(); ++i)
			h = h * 31 + static_cast<int>(str[i]);
		return h;
	}

	void hashFile(string filename) {
		if (!isExistsFile(filename)) {
			ShowAllFiles();
			throw "File not found";
		}

		for (size_t i = 0; i < file_count; i++)
		{
			if (files[i]->GetFilename() == filename) {
				auto hash = HashCode(files[i]->GetContent());
				files[i]->SetContent(to_string(hash));
			}
		}
	}

	void ShowAllFiles() {
		cout << "\n\n* * * * * * * " << _discName << " properties * * * * * * *" << endl;
		cout << "Current size: " << current_size << endl;
		cout << "Max size: " << max_size << endl;
		cout << "File count: " << file_count << endl;
		for (size_t i = 0; i < file_count; i++)
		{
			cout << "\n- - - - - - - File properties " << i + 1 << " - - - - - - -" << endl;
			cout << "File name: " << files[i]->GetFilename() << endl;
			cout << "File size: " << files[i]->GetFileSize() << endl;
			cout << "File content: " << files[i]->GetContent() << endl;
			cout << "File created time: " << files[i]->GetCreatedTime();
			cout << "File changed time: " << files[i]->GetChangedTime() << endl;

		}
	}

	virtual ~Disc() = 0
	{

	}
};


class HDD :public Disc {
public:
	HDD(const string& name) :Disc(name)
	{

	}
	void createFile(string filename, string content = "", string createdTime = __TIME__) override {
		for (size_t i = 0; i < content.size(); i++)
		{
			Sleep(100);
		}
		cout << "File created successfuly" << endl;
		Disc::createFile(filename, content);
	}

	void setFileContent(string filename, string content = "", string changedTime = __TIME__) override {
		for (size_t i = 0; i < content.size(); i++)
		{
			Sleep(100);
		}
		cout << "\nThe contents of the " << filename << " file have been successfully modified.";
	}



};

class SSD :public Disc {
public:
	SSD(const string& name) :Disc(name)
	{

	}
	void createFile(string filename, string content = "", string createdTime = __TIME__) override {
		for (size_t i = 0; i < content.size(); i++)
		{
			Sleep(10);
		}
		cout << "File created successfuly" << endl;
		Disc::createFile(filename, content);
	}

	void setFileContent(string filename, string content = "", string changedTime = __TIME__) override {
		for (size_t i = 0; i < content.size(); i++)
		{
			Sleep(10);
		}
		cout << "\nThe contents of the " << filename << " file have been successfully modified.";
	}


};

const double Disc::max_size = 100;

class OperationSystem {
	OperationSystem() {}
public:
	static void controlPC();
};

void OperationSystem::controlPC() {
	SSD ssd("SSD");
	HDD hdd("HDD");
	try
	{
		ssd.createFile("newtext.txt", "hello");
		ssd.createFile("newtext1.txt", "world");
		hdd.createFile("newtext2.txt", "hakuna");
		hdd.createFile("newtext3.txt", "matata");
		hdd.createFile("newtext4.txt", "programmer");
		hdd.setFileContent("newtext2.txt", "hello world");
		hdd.setFileContent("newtext.txt", "dfghgahsd");
		ssd.setFileContent("newtext1.txt", "asdasdasdewwe");
		//hdd.getFile("newtext.txt");
		//hdd.deleteFileContent("newtext.txt");
		//hdd.deleteFileByFilename("newtext.txt");
		//hdd.updateFilename("newtext3.txt", "newname.txt");
		//hdd.hashFile("newtext.txt");

		ssd.ShowAllFiles();
		hdd.ShowAllFiles();

	}
	catch (const char* ex)
	{
		cout << "\nException : " << ex << endl;
	}
}

void main() {
	OperationSystem::controlPC();

}

