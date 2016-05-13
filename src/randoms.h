
#ifndef		_RANDOMS
#define		_RANDOMS

#include <math.h>
#include <stdlib.h>
#include <time.h>
/************* adopted from NUMERICAL RECEPIES pg/11/2004 ********************/

#define R_IM            259200
#define R_IA            7141
#define R_IC            54773
/* #define R_IM            714025 */
/* #define R_IA            4096 */
/* #define R_IC            150889 */


struct Rdat3
{
    int		next;
    int		nextp;
    int	ma[56];
    int		iff;
    double	scale;
    double	offs;
    char	label[20];
};

struct Rdati
{
    int		seed;
    int		scale;
    int		offs;
    char	label[20];
};

struct Rdatg
{
    struct Rdat3	xr3, yr3;
    double		mean, sig2;
    int			set;
    double		next;
    char		label[20];
};

void		Rinit3( struct Rdat3 *, char *, double, double ),
		Rinitg( struct Rdatg *, char *, double, double ),
		Riniti( struct Rdati *, char *, int, int ),
		RinitGenerator ( double );

double		Rran3( struct Rdat3 * ),
		Rgauss( struct Rdatg * );

int		Rrani( struct Rdati * );

#endif
