#include "GribDecoder.h"
#include <iostream>
#include <vector>

using namespace std;

GribDecoder::GribDecoder()
{}

void GribDecoder::displayInfo()
{
	char byte1;
	char byte2;
	char byte3;
	char byte4;

	cout << "==========================\n";
	cout << "== General Message Info ==\n";
	cout << "==========================\n";


	while (!grib.eof())
	{
		grib.read(&byte1, 1);
		if (byte1 == 'G')
		{
			grib.read(&byte2, 1);
			grib.read(&byte3, 1);
			grib.read(&byte4, 1);
			if (byte2 == 'R' && byte3 == 'I' && byte4 == 'B')
			{
				start_section0 = static_cast<uint32_t>(grib.tellg()) - 4;
				cout << "Grib was found at pos: " << start_section0 << "\n";
				
				grib.seekg(start_section0);
				break;
			}
		}
	}

	while (!grib.eof())
	{
		grib.read(&byte1, 1);
		if (byte1 == '7')
		{
			grib.read(&byte2, 1);
			grib.read(&byte3, 1);
			grib.read(&byte4, 1);
			if (byte2 == '7' && byte3 == '7' && byte4 == '7')
			{
				cout << "7777 was found at pos: " << static_cast<uint32_t>(grib.tellg()) << "\n";
				end_7777 = static_cast<uint32_t>(grib.tellg());
				break;
			}
		}
	}

	grib.seekg(start_section0 + 4);


	grib.read(&byte1, 1);
	grib.read(&byte2, 1);
	grib.read(&byte3, 1);

	int messageL = 0;
	messageL = ((int)byte1 << 16) | ((int)byte2 << 8) | (int)byte3 & 0xff;

	grib >> byte4;

	uint32_t length2 = end_7777 - (start_section0 - 4);
	
	cout << "Length between end of 'Grib' and start of '7777': " << length2 << " bytes\n";
	cout << "Whole Message length:: " << messageL << " bytes\n";
	cout << endl;
}

void GribDecoder::s1()
{

	cout << "==========================\n";
	cout << "===== Section 1 Data =====\n";
	cout << "==========================\n";

	start_section1 = grib.tellg();

	char byte1;
	char byte2;
	char byte3;

	//Length in octets of the Product Definition Section 
	grib.read(&byte1, 1);
	grib.read(&byte2, 1);
	grib.read(&byte3, 1);

	int s1Length = 0;
	s1Length = (int)byte1 << 16 | (int)byte2 << 8 | (int)byte3 & 0xff;
	cout << "Section 1 length: " << s1Length << "\n";

	//Parameter Table Version number
	grib.seekg(start_section1 + 3);
	grib.read(&byte1, 1);
	int tVersion = static_cast<int>(byte1);
	cout << "Table Version: " << tVersion << "\n";

	//Identification of center
	grib.seekg(start_section1 + 4);
	grib.read(&byte1, 1);
	int ID = static_cast<int>(byte1);
	//cout << ID;
	cout << "ID of center = " << ID << ": US National Weather Service - NCEP (WMC) \n";

	//Generating process ID number
	grib.seekg(start_section1 + 5);
	grib.read(&byte1, 1);
	int IDn = static_cast<int>(byte1);
	//cout << IDn;
	cout << "Process ID number = " << IDn << ": Analysis from GFS(Global Forecast System) \n";

	//Grid Identification
	grib.seekg(start_section1 + 6);
	grib.read(&byte1, 1);
	int gID = static_cast<int>(byte1);
	//cout << gID;
	cout << "Grid Identification: " << gID << "\n";

	//Flag specifying the presence or absence of a GDS or a BMS
	grib.seekg(start_section1 + 7);
	uint8_t flag = static_cast<uint8_t>(byte1);
	bool gds_in = flag & 0b00000001;
	bool bms_in = flag & 0b00000010;
	cout << "GDS and BMS: ";
	if (gds_in)
		cout << "GDS Included ";
	else
		cout << "GDS Omitted ";

	if (bms_in)
		cout << "BMS Included\n";
	else
		cout << "BMS Omitted\n";

	//Indicator of parameter and units
	grib.seekg(start_section1 + 8);
	grib.read(&byte1, 1);
	int pIndicator = static_cast<int>(byte1);
	cout << "Indicator of parameter and units: " << pIndicator << "\n";

	//Indicator of type of level or layer
	grib.seekg(start_section1 + 9);
	grib.read(&byte1, 1);
	int Indicatorl = static_cast<int>(byte1);
	cout << "Indicator of type of level or layer: " << Indicatorl << "\n";

	// Height, pressure, etc. of the level or layer
	grib.seekg(start_section1 + 10);
	grib.read(&byte1, 1);
	grib.read(&byte2, 1);
	unsigned short value = (static_cast<unsigned char>(byte1) << 8) | static_cast<unsigned char>(byte2);
	std::cout << "Height, pressure, etc. of the level or layer: " << value << "\n";


	// 	Year of century | Month of year | Day of month | Hour of day | Minute of hour 
	int y, m, d, h, min;

	grib.seekg(start_section1 + 12);
	grib.read(&byte1, 1);
	y = (int)byte1;

	grib.seekg(start_section1 + 13);
	grib.read(&byte1, 1);
	m = (int)byte1;

	grib.seekg(start_section1 + 14);
	grib.read(&byte1, 1);
	d = (int)byte1;

	grib.seekg(start_section1 + 15);
	grib.read(&byte1, 1);
	h = (int)byte1;

	grib.seekg(start_section1 + 16);
	grib.read(&byte1, 1);
	min = (int)byte1;

	cout << "YY/MM/DD|HH:MM " << y << "/" << m << "/" << d << "|" << h << min << ":00" "\n";

	// Forecast time unit
	grib.seekg(start_section1 + 17);
	grib.read(&byte1, 1);
	int UnitT = static_cast<int>(byte1);
	cout << "Forecast time unit: " << UnitT << "\n";

	// P1 - Period of time
	grib.seekg(start_section1 + 18);
	grib.read(&byte1, 1);
	int p1 = static_cast<int>(byte1);
	cout << "P1 - Period of time  : " << p1 << "\n";

	// P2 - Period of time (Number of time units)

	grib.seekg(start_section1 + 19);

	grib.read(&byte1, 1);
	int p2 = static_cast<int>(byte1);
	cout << "P2 - Period of time  : " << p2 << "\n";

	// Time range indicator
	grib.seekg(start_section1 + 20);
	grib.read(&byte1, 1);
	int range = static_cast<int>(byte1);
	cout << "Time range indicator:  " << range << "\n";


	// Number included in average
	grib.seekg(start_section1 + 21);
	grib.read(&byte1, 1);
	int lastRow = static_cast<int>(byte1);
	cout << "Numbers for last row: " << lastRow << "\n";

	// Number Missing from averages or accumulations
	grib.seekg(start_section1 + 23);
	grib.read(&byte1, 1);
	std::cout << "Number missing from averages or accumulation: " << static_cast<int>(byte1) << "\n";

	//  Century of Initial (Reference) time (=20 until Jan. 1, 2001) 
	grib.seekg(start_section1 + 24);
	grib.read(&byte1, 1);
	int century = static_cast<int>(byte1);
	cout << "Reference century: " << century << "\n";

	// Identification of sub-center
	grib.seekg(start_section1 + 25);
	grib.read(&byte1, 1);
	int sID = static_cast<int>(byte1);
	cout << "Identification of sub Center: " << sID << "\n";

	// The decimal scale factor D A negative value is indicated by setting the high order bit (bit No. 1) in octet 27 to 1 (on). 
	grib.seekg(start_section1 + 26);
	grib.read(&byte1, 1);
	grib.read(&byte2, 1);
	unsigned short scale = (static_cast<unsigned char>(byte1) << 8) | static_cast<unsigned char>(byte2);
	std::cout << "Decimal Scale factor: " << scale << "\n";

	grib.seekg(start_section1 + s1Length);

	cout << endl;
}

void GribDecoder::s2()
{
	char byte1, byte2, byte3;
	start_section2 = grib.tellg();
	grib.read(&byte1, 1);
	grib.read(&byte2, 1);
	grib.read(&byte3, 1);

	cout << "==========================\n";
	cout << "===== Section 2 Data =====\n";
	cout << "==========================\n";

	// Length in octets of the Grid Description Section
	int32_t s2Lenght = (int)byte1 << 16 | (int)byte2 << 8 | (int)byte3 & 0xff;
	cout << "Section 2 length: " << s2Lenght << "\n";

	// 	NV, the number of vertical coordinate parameters
	grib.seekg(start_section2 + 3);
	grib.read(&byte1, 1);
	cout << "Number of Vertical Coordinates: " << static_cast<int>(byte1) << "\n";

	// PV, the location (octet number) of the list of vertical coordinate parameters
	grib.seekg(start_section2 + 4);
	grib.read(&byte1, 1);
	cout << "Octet number: " << static_cast<int>(byte1) << "\n";

	// Data representation type 
	grib.seekg(start_section2 + 5);
	grib.read(&byte1, 1);
	cout << "Data representation type: " << static_cast<int>(byte1) << " | Latitude/Longitude Grid - Equidistant Cylindrical or Plate Carree projection \n";

	// Ni - No. of points along a latitude circle
	grib.seekg(start_section2+6);
	grib.read(&byte1, 1);
	grib.read(&byte2, 1);
	unsigned short circle = (static_cast<unsigned char>(byte1) << 8) | static_cast<unsigned char>(byte2);
	cout << "Numbers of points along a latitude circle: " << circle << "\n";

	// Nj - No. of points along a longitude meridian
	grib.seekg(start_section2 + 8);
	grib.read(&byte1, 1);
	grib.read(&byte2, 1);
	unsigned short meridian = (static_cast<unsigned char>(byte1) << 8) | static_cast<unsigned char>(byte2);
	cout << "Numbers of points along a longitude meridian: " << meridian << "\n";

	// La1 - latitude of first grid point; units: millidegrees (degrees x 1000); values limited to range 0 - 90,000; bit 1 (leftmost) set to 1 for south latitude
	grib.seekg(start_section2 + 10);
	grib.read(&byte1, 1);
	grib.read(&byte2, 1);
	grib.read(&byte3, 1);
	unsigned short latitude = (static_cast<unsigned char>(byte1) << 16) | (static_cast<unsigned char>(byte2) << 8) | static_cast<unsigned char>(byte3);
	double degre = static_cast<double>(latitude) / 1000.0;
	cout << "Latitude: " << degre << " degrees\n";

	// Lo1 - longitude of first grid point; units: millidegrees (degrees x 1000); values limited to range 0 - 360,000; bit 1 (leftmost) set to 1 for west longitude

	grib.seekg(start_section2 + 13);
	grib.read(&byte1, 1);
	grib.read(&byte2, 1);
	grib.read(&byte3, 1);
	unsigned short longtitude = (static_cast<unsigned char>(byte1) << 16) | (static_cast<unsigned char>(byte2) << 8) | static_cast<unsigned char>(byte3);
	double degree = static_cast<double>(longtitude) / 1000.0;
	cout << "Longtitude: " << degree << " degrees\n";

	grib.seekg(start_section2 + 16);
	grib.read(&byte1, 1);
	string flags;
	if ((byte1 & 0b00000010) != 0) { flags += "Directional increment given "; }
	else { flags += "Directional increment not given "; }

	if ((byte1 & 0b00000001) != 0) { flags += "and Earth assumed oblate spheroid"; }
	else { flags += "and Earth assumed spherical"; }
	cout << "Resolution and component flags: " << flags << "\n";

	// La2 - Latitude of last grid point
	grib.seekg(start_section2 + 17);
	grib.read(&byte1, 1);
	grib.read(&byte2, 1);
	grib.read(&byte3, 1);
	unsigned int latitude1 = (static_cast<unsigned char>(byte1) << 16) | (static_cast<unsigned char>(byte2) << 8) | static_cast<unsigned char>(byte3);
	double degree1 = static_cast<double>(latitude1) / 1000.0;
	cout << "Latitude of last grid point: " << degree1 << " degrees\n";

	// Lo2 - Longitude of last grid point (same units, value range, and bit 1 as Lo1)
	grib.seekg(start_section2 + 20);
	grib.read(&byte1, 1);
	grib.read(&byte2, 1);
	grib.read(&byte3, 1);
	unsigned int longtitude1 = (static_cast<unsigned char>(byte1) << 16) | (static_cast<unsigned char>(byte2) << 8) | static_cast<unsigned char>(byte3);
	double degre1 = static_cast<double>(longtitude1) / 1000.0;
	cout << "Latitude of last grid point: " << degre1 << " degrees\n";

	// Scanning mode flags

	grib.seekg(start_section2 + 27);
	grib.read(&byte1, 1);

	bool i_direction_minus = byte1 & (1 << 0);
	bool j_direction_minus = byte1 & (1 << 1);
	bool i_consecutive = byte1 & (1 << 4);
	cout << "Scanning mode flags: Points scan in ";
	if (i_direction_minus) { cout << "-i and "; }
	else { cout << "+i and "; }

	if (j_direction_minus) { cout << "+j "; }
	else { cout << "-j "; }

	cout << "direction. Fortran: (";

	if (i_consecutive) { cout << "J,I)"; }
	else { cout << "I,J)"; }
	cout << endl;

	
	grib.seekg(start_section2 + s2Lenght);

	// Liczenie Dlugosci Sekcji 3
	//start_section3 = grib.tellg();
	//grib.read(&byte1, 1);
	//grib.read(&byte2, 1);
	//grib.read(&byte3, 1);
	//int32_t s3Length = (int)byte1 << 16 | (int)byte2 << 8 | (int)byte3 & 0xff;
	//grib.seekg(start_section3 + s3Length);

	cout << endl;
}

 void GribDecoder::s4()
{
	cout << "==========================\n";
	cout << "===== Section 4 Data =====\n";
	cout << "==========================\n";

	start_section4 = grib.tellg();

	char byte1, byte2, byte3;

	//	Length in octets of binary data section
	grib.read(&byte1, 1);
	grib.read(&byte2, 1);
	grib.read(&byte3, 1);

	int32_t s4Lenght = (int)byte1 << 16 | (int)byte2 << 8 | (int)byte3 & 0xff;
	cout << "Dlugosc sekcji 4: " << s4Lenght << "\n";

	// Bits 1 through 4: Flag (See Table 11) | Bits 5 through 8: Number of unused bits at end of Section 4.
	grib.seekg(start_section4 + 3);
	grib.read(&byte1, 1);
	int decodeFlag = static_cast<unsigned char>(byte1);
	cout << "Flag to decode: " << decodeFlag << "\n";

	// The binary scale factor
	grib.seekg(start_section4 + 4);
	grib.read(&byte1, 1);
	grib.read(&byte2, 1);
	short binaryScaleFactor = (static_cast<unsigned char>(byte1) << 8) | static_cast<unsigned char>(byte2);
	cout << "Binary Scale Factor: " << binaryScaleFactor << "\n";

	// Number of bits into which a datum point is packed
	grib.seekg(start_section4 + 10);
	grib.read(&byte1, 1);
	int nBits = static_cast<unsigned char>(byte1);
	cout << "Number of bits " << nBits << "\n";
}


void GribDecoder::readGribFile(const std::string& filename)
{
	grib.open(filename, ios::binary | ios::in);
	if (!grib.is_open()) { cout << "Nie udalo sie otworzyc pliku!\n"; return; }

	displayInfo(); // s0
	s1();
	s2();
	s4();

	grib.close();
}