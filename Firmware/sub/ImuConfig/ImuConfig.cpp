/**
 * @file ImuConfig.cpp
 * @author Dan Oates (WPI Class of 2020)
 */
#include <ImuConfig.h>

/**
 * Namespace Definitions
 */
namespace ImuConfig
{
#if ES3011_BOT_ID == 0
	const float gyr_x_cal = -0.0734113680000f;
	const float gyr_y_cal = +0.0269964600000f; 
	const float gyr_z_cal = +0.0101013720000f;
	const float gyr_x_var = 0.00000228365390f;
	const float gyr_y_var = 0.00000283399940f;
	const float gyr_z_var = 0.00000185751390f;
	const float acc_x_var = 0.00112722710000f;
	const float acc_y_var = 0.00090445904000f;
	const float acc_z_var = 0.00162959800000f;
#elif ES3011_BOT_ID == 1
	const float gyr_x_cal = +0.0003701793300f;
	const float gyr_y_cal = -0.0758841110000f;
	const float gyr_z_cal = -0.0199697190000f;
	const float gyr_x_var = 0.00000215523980f;
	const float gyr_y_var = 0.00000358851340f;
	const float gyr_z_var = 0.00000316657540f;
	const float acc_x_var = 0.00077350164000f;
	const float acc_y_var = 0.00102186380000f;
	const float acc_z_var = 0.00129695180000f;
#elif ES3011_BOT_ID == 2
	const float gyr_x_cal = -0.0719998850000f;
	const float gyr_y_cal = +0.0160055890000f;
	const float gyr_z_cal = +0.1958248800000f;
	const float gyr_x_var = 0.00000883372190f;
	const float gyr_y_var = 0.00000227666920f;
	const float gyr_z_var = 0.00000215141040f;
	const float acc_x_var = 0.00101798410000f;
	const float acc_y_var = 0.00107184890000f;
	const float acc_z_var = 0.00253210360000f;
#elif ES3011_BOT_ID == 3
	const float gyr_x_cal = -0.0461459160000f;
	const float gyr_y_cal = +0.0048815743000f;
	const float gyr_z_cal = -0.0158524590000f;
	const float gyr_x_var = 0.00000366372610f;
	const float gyr_y_var = 0.00000603395980f;
	const float gyr_z_var = 0.00000371762870f;
	const float acc_x_var = 0.00102829980000f;
	const float acc_y_var = 0.00118071350000f;
	const float acc_z_var = 0.00219102880000f;
#elif ES3011_BOT_ID <= 20
	#warning "Not calibrated!"
	const float gyr_x_cal = 0.0f;
	const float gyr_y_cal = 0.0f;
	const float gyr_z_cal = 0.0f;
	const float gyr_x_var = 1.0f;
	const float gyr_y_var = 1.0f;
	const float gyr_z_var = 1.0f;
	const float acc_x_var = 1.0f;
	const float acc_y_var = 1.0f;
	const float acc_z_var = 1.0f;
#else
#error Must define ES3011_BOT_ID in range [0, 20]
#endif
}