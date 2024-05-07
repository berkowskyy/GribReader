#ifndef _GRIBDECODER_H
#define _GRIBDECODER_H

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class GribDecoder
{

private:
	/// <summary>
	/// Wej�cie/wyj�cie pliku - obs�uga pliku grib
	/// </summary>
	fstream grib;
	/// <summary>
	/// Rozpocz�cie sekcji 0 w pliku .grib
	/// </summary>
	uint32_t start_section0 = 0;
	/// <summary>
	/// Rozpocz�cie sekcji 1 w pliku .grib
	/// </summary>
	uint32_t start_section1 = 0;
	/// <summary>
	/// Rozpocz�cie sekcji 2 w pliku .grib
	/// </summary>
	uint32_t start_section2 = 0;
	/// <summary>
	/// Rozpocz�cie sekcji 3 w pliku .grib
	/// </summary>
	uint32_t start_section3 = 0;
	/// <summary>
	/// Rozpocz�cie sekcji 4 w pliku .grib
	/// </summary>
	uint32_t start_section4 = 0;
	/// <summary>
	/// Zmienna wymagana do znalezienie ko�ca wyszukania wyrazu "7777"
	/// </summary>
	uint32_t end_7777 = 0;

	/// <summary>
	/// Metoda wy�wietalaj�ca og�lne informacje o pliku .grib z Sekcji 0
	/// </summary>
	void displayInfo();
	/// <summary>
	/// Metoda odczytuj�ca informacje z pliku .grib z Sekcji 1
	/// </summary>
	void s1();
	/// <summary>
	/// Metoda odczytuj�ca informacje z pliku .grib z Sekcji 2
	/// </summary>
	void s2();
	/// <summary>
	/// Metoda odczytuj�ca informacje z pliku .grib z Sekcji 4
	/// </summary>
	void s4();
public:
	/// <summary>
	/// Udost�pnienie klasy GribDecoder
	/// </summary>
	GribDecoder();
	/// <summary>
	/// Publiczna metoda odczytuj�ca wszystkie informacje o pliku .grib
	/// </summary>
	/// <param name="filename">Zmienna potrzebna do wczytania konkretnego pliku .grib</param>
	void readGribFile(const std::string& filename);
};

#endif // GRIBDECODER_H