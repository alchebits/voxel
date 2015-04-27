#ifndef UTIL_H_
#define UTIL_H_

#include <iostream>
#include <cstring>
#include <string>
#include <cstdlib>
#include <fstream>

class StringTokenizer{
protected:
	std::string m_strSeparators; // ciag znakow, ktore są uznawane za odzielajace kolejne slowa (tokeny)
	std::string m_strStringCopy; // string do utworzenia kopii, jesli taki bedzie ustawiony tryb
	const std::string* m_pstrWorker; //  wskaznik ustawiany na oryginal albo kopie stringa ( w zal. od trybu)
	bool m_bStrCopy; // jesli true, w klasie na poczatku robiona jest kopia
	unsigned m_uTmpIndex;
	unsigned m_uTokenTail;
	bool m_bLoaded;
public:

	~StringTokenizer();
	explicit StringTokenizer(
			bool makeCopy = true,
			const std::string separators = " \n\t" // spacja i enter i tab standardowo sa uznawane za separatory
			);
	explicit StringTokenizer(
			const std::string& str,
			bool makeCopy = true,
			const std::string separators = " \n\t" // spacja i enter i tab standardowo sa uznawane za separatory
			);
	void loadString(const std::string& str);
	void reset(); // powoduje, ze tokeny sa zwracane od samego poczatku
	bool nextToken(std::string& resultToken);
	bool nextToken(std::string& resultToken, unsigned& resultSepBeforeToken);
private:
	// sprawdza, czy znak w podanym indexie stringa jest separatorem, czy nie jest
	bool isSeparator(unsigned index);
};


class IOWTextFileReader{
protected:

	std::ifstream m_ifsFile; // uchwyt do pliku
	std::string m_strFilePath; // przechowuje nazwe pliku

public:

	~IOWTextFileReader();
	/**
	 * Konstruktor, tworzy obiekt, mozna potem otworzyc jakis plik tekstowy za pomoca operacji openFile
	 */
	IOWTextFileReader();

	/**
	 * Tworzy obiekt i odrazu otwiera jakis plik (lub nie, jesli nie ma go w podanej sciezce)
	 * @param filePath -Sciezka do pliku tekstowego (multiplatformowa)
	 */
	IOWTextFileReader(const char* filePath);

	/**
	 * Jesli plik jest otwarty, zwraca kolejna linie pliku, jesli plik sie zakonczy, zamyka go
	 * @param nextLineStr -ref na obiekt string, do ktorego zostanie zapisana kolejna linia pliku
	 * @return -true -wczytano kolejną linie pliku, false -> Nie ma otwartego pliku, albo poprzedni sie skonczyl
	 */
	bool nextLine(std::string& nextLineStr);

	/**
	 * Otwiera plik do czytania
	 * @param filePath -Sciezka do pliku
	 * @return -true, otworzono plik, false- nie otworzono bo np brak pliku.
	 */
	bool openFile(const char* filePath);

	/**
	 * Zamyka bierzacy plik
	 * @return true- jesli bylo co zamykac, false, jesli nie bylo
	 */
	bool closeFile();

	/**
	 * True jesli znaleziono plik i jest otworzony
	 * @return
	 */
	bool isOpened();

private:
};

// Aktualnie klasa prymitywnie zawsze bedzie przegladala tablice parametrow,
// Na przyszlosc nalezy w konstruktorze napisac tworzenie mapy "parametr" : "wartosc"
// i to z tej mapy powinny korzystac metody (dla lepszej wydajnosci)
class ParameterParser{
private:
	static const int STRCMP_RES_EQUALS = 0;
	static const int STRCMP_RES_FIRST_LOWER = -1;
	static const int STRCMP_RES_FIRST_BIGGER = 1;

	int argc;
	char** argv;
public:
	explicit ParameterParser(int argc, char** argv);

	bool haveFlag(const std::string& flag);
	bool haveStringParameter(const std::string& parameter);
	bool haveDoubleParameter(const std::string& parameter);
	std::string getStringParameter(const std::string& parameter, const std::string& defaultParam);
	double getDoubleParameter(const std::string& parameter, double defaultParam);

private:
	int getFlagID(const std::string& flag);
	bool paramExistsOnID(int id);
	bool isDoubleInString(const std::string& str);
};


#endif
