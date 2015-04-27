#include "../include/util.h"

StringTokenizer::~StringTokenizer(){ }

StringTokenizer::StringTokenizer(const std::string& str, bool makeCopy, const std::string separators )
{
	m_strSeparators = separators;
	m_bStrCopy = makeCopy;

	this->loadString(str);
	m_bLoaded = true;
}

StringTokenizer::StringTokenizer( bool makeCopy, const std::string separators )
{
	m_strSeparators = separators;

	m_bStrCopy = makeCopy;
	m_bLoaded = false;
}

void StringTokenizer::loadString(const std::string& str){
	if(m_bStrCopy){
		m_strStringCopy = str;
		m_pstrWorker = &m_strStringCopy;
	}else{
		m_pstrWorker = &str;
	}
	this->reset();
	m_bLoaded = true;
}

// powoduje, ze tokeny sa zwracane od samego poczatku
void StringTokenizer::reset(){
	m_uTmpIndex = 0;
}

bool StringTokenizer::nextToken(std::string& resultToken){
	if(!m_bLoaded)
		return false;

	const std::string& string = *m_pstrWorker;

	while( m_uTmpIndex != m_pstrWorker->size() && this->isSeparator(m_uTmpIndex) )
		++m_uTmpIndex;

	if(m_uTmpIndex == string.size())
		return false;

	m_uTokenTail = m_uTmpIndex;

	while( m_uTokenTail != m_pstrWorker->size() && !this->isSeparator(m_uTokenTail) )
		++m_uTokenTail;

	resultToken = m_pstrWorker->substr(m_uTmpIndex, m_uTokenTail - m_uTmpIndex );
	m_uTmpIndex = m_uTokenTail;
	return true;
}

// Zwraca kolejny token i liczbe separatorow przed nim. -1 oznacza koniec
bool StringTokenizer::nextToken(std::string& resultToken, unsigned& resultSepBeforeToken){
	if(!m_bLoaded)
		return false;

	// ulatwienie sobie skladni w dalszym kodzie
	const std::string& string = *m_pstrWorker;
	resultSepBeforeToken = 0;

	// teraz wyszukanie tokenu:
	// index na poczatek kolejnego tokenu
	while( m_uTmpIndex != m_pstrWorker->size() && this->isSeparator(m_uTmpIndex) ){
		++m_uTmpIndex;
		resultSepBeforeToken++;
	}

	// walidacja, czy juz nie zostal przejzany caly napis
	if(m_uTmpIndex == string.size())
		return false;

	m_uTokenTail = m_uTmpIndex;

	// teraz ustawienie m_uBuf1 na koniec tokena;
	while( m_uTokenTail != m_pstrWorker->size() && !this->isSeparator(m_uTokenTail) )
		++m_uTokenTail;

	// teraz skopiowanie tokena
	resultToken = m_pstrWorker->substr(m_uTmpIndex, m_uTokenTail - m_uTmpIndex );
	m_uTmpIndex = m_uTokenTail; // ustawienie aktualnego indexu w stringu do pobierania kolejnych tokenow
	return true;
}

// Funkcja sprawdza, czy znak o podanym indexie jest spearatorem
bool StringTokenizer::isSeparator(unsigned index){

	// walidacja
	if(index >= m_pstrWorker->size())
		return false;

	bool flag = false; // flaga, ktora zostanie ustawiona, jesli znak jest separatorem
	for( unsigned i = 0; i < m_strSeparators.size() ; i++ ){
		if( m_pstrWorker->at(index) == m_strSeparators.at(i) ){
			flag = true;
			break;
		}
	}

	return flag;
}


// Destruktor, zamyka plik, jesli jakis jest otwarty
IOWTextFileReader::~IOWTextFileReader() {
	if( m_ifsFile.is_open() )
		m_ifsFile.close();
}

// Konstruktor bezparametrowy, nie robi nic
IOWTextFileReader::IOWTextFileReader(){}

// Konstruktor ze sciezka, otwiera odrazu plik
IOWTextFileReader::IOWTextFileReader(const char* filePath){
	this->openFile(filePath);
}

// Zwraca kolejna linie z pliku tekstowego.
bool IOWTextFileReader::nextLine(std::string& nextLineStr){

	// brak otworzonego pliku, zwracamy false
	if(!m_ifsFile.is_open())
		return false;

	// jesli koniec pliku juz mamy to zamykamy plik, i zwracamy "" oraz false
	if(m_ifsFile.eof())
	{
		m_ifsFile.close();
		nextLineStr = "";
		return false;
	}

	// Jesli jednak jeszcze jest co wczytywac z pliku, to wczytujemy
	getline(m_ifsFile, nextLineStr);

	return true;
}
bool IOWTextFileReader::openFile(const char* filePath){

	// jesli jakis byl otworzony, to automatycznie go zamykamy
	if(m_ifsFile.is_open())
		m_ifsFile.close();

	char pathBuffer[300];
#ifdef __ANDROID__
	sprintf( pathBuffer, "%s%s", IOW_EP3D_SDCARD_PATH, filePath ); // utworzenie globalnej sciezki do pliku na karcie sd
#else
	sprintf( pathBuffer, "%s",  filePath ); // utworzenie globalnej sciezki do pliku na karcie sd
#endif

	m_ifsFile.open( pathBuffer ); // otworzenie pliku do czytania

	// jesli udalo sie otworzyc, zwracamy true, jesli nie, false
	if(m_ifsFile.is_open())
		return true;
	else
	{
#ifdef ERROR_DEBUG_FILE_WRAPPER_HPP_
		char errInfo[150];
		sprintf(errInfo, "Nie znaleziono pliku: %s", pathBuffer);
		dbgError(__FILE__,__LINE__,errInfo);
#endif
		return false;
	}
}

// Zamyka plik, jesli zamknal, zwraca true
bool IOWTextFileReader::closeFile(){
	if(m_ifsFile.is_open()){
		m_ifsFile.close();
		return true;
	}
	else
		return false;
}

// zwraca true jesli plik jest otwrzony
bool IOWTextFileReader::isOpened(){

	if(m_ifsFile.is_open())
		return true;
	else
		return false;
}


ParameterParser::ParameterParser(int argc, char** argv) {
	this->argc = argc;
	this->argv = argv;
}

bool ParameterParser::haveFlag(const std::string&  flag){
	int flagID = this->getFlagID(flag);
	return (flagID > 0) ? true : false;
}
bool ParameterParser::haveStringParameter(const std::string&  parameter){
	int parameterID = this->getFlagID(parameter);
	if( parameterID != -1)
	if( paramExistsOnID(parameterID + 1) ){
		return true;
	}
	return false;
}
bool ParameterParser::haveDoubleParameter(const std::string&  parameter){
	int parameterID = this->getFlagID(parameter);
	if( parameterID != -1)
	if( paramExistsOnID(parameterID + 1) ){
		bool isDouble = this->isDoubleInString( std::string(argv[parameterID+1]) );
		return isDouble;
	}
	return false;
}
std::string ParameterParser::getStringParameter(const std::string& parameter, const std::string& defaultParam){
	if( haveStringParameter(parameter) ){
		int paramID = this->getFlagID(parameter);
		return std::string(argv[paramID + 1]);
	}
	return defaultParam;
}
double ParameterParser::getDoubleParameter(const std::string& parameter, double defaultParam){
	if( haveStringParameter(parameter) ){
		int paramID = this->getFlagID(parameter);
		char* p;
		return strtod( argv[paramID+1], &p );
	}
	return defaultParam;
}

int ParameterParser::getFlagID(const std::string& flag){
	for(int i = 0 ; i < argc ; ++i){
		int cmpResult = strcmp( argv[i], flag.c_str() );
		if(cmpResult == STRCMP_RES_EQUALS){
			return i;
		}
	}
	return -1;
}
bool ParameterParser::isDoubleInString(const std::string& str){
	char* p;
	double val = strtod( str.c_str(), &p );
	val += 1; // aby usunac warning kompilacji
	if ( *p == 0 ) {
	   return true;
	}else{
		return false;
	}
}

bool ParameterParser::paramExistsOnID(int id){
	if(id < argc){
		return true;
	}
	return false;
}

