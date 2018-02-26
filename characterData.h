//Colour Values
unsigned int colorDBlue []  = {6, 4, 24};
unsigned int colorGreen []  = {2, 23, 3};
unsigned int colorRed []  = {28, 0, 0};
unsigned int colorOrange []  = {31, 16, 0};
unsigned int colorDarkRed [] = {19, 2, 0};
unsigned int colorLBlue []  = {2, 15, 25};
unsigned int colorYellow []  = {26, 30, 3};
unsigned int colorBlack[] = {0, 0, 0};
unsigned int colorWhite[] = {31, 31, 31};
unsigned int laserTip[] = {30, 10, 10};
unsigned int colorGrey [] = {13, 13, 13};
unsigned int colorLGrey [] = {20, 20, 20};

//Tile Data
unsigned int longShip1[64] =
	{0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 4, 1, 1, 1, 1, 1, 1,
	 0, 4, 1, 1, 5, 5, 1, 5,
	 0, 4, 1, 1, 5, 5, 1, 5,
	 0, 4, 1, 1, 1, 1, 1, 1,
	 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0};
	 
unsigned int longShip2[64] =
	{0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0,
	 1, 1, 4, 0, 0, 0, 0, 0,
	 5, 1, 4, 4, 0, 0, 0, 0,
	 5, 1, 4, 4, 4, 0, 0, 0,
	 1, 1, 1, 1, 1, 0, 0, 0,
	 0, 0, 2, 2, 0, 0, 0, 0,
	 0, 0, 2, 2, 2, 2, 0, 0};
	 
unsigned int longShipGo[64] =
	{0, 0, 0, 0, 0, 0, 0, 0,
	 3, 0, 0, 0, 0, 0, 0, 0,
	 0, 3, 1, 1, 1, 1, 1, 1,
	 3, 3, 1, 1, 5, 5, 1, 5,
	 3, 3, 1, 1, 5, 5, 1, 5,
	 0, 3, 1, 1, 1, 1, 1, 1,
	 3, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0};
	
unsigned int bulletDraw[64] =
	{0, 0, 0, 0, 0, 0, 0, 0,
	 0, 5, 5, 0, 0, 5, 5, 0,
	 0, 5, 2, 2, 2, 2, 5, 0,
	 0, 0, 2, 3, 3, 2, 0, 0,
	 0, 0, 2, 3, 3, 2, 0, 0,
	 0, 5, 2, 2, 2, 2, 5, 0,
	 0, 5, 5, 0, 0, 5, 5, 0,
	 0, 0, 0, 0, 0, 0, 0, 0};
	 
unsigned int laser[256] = 
	{0, 0, 0, 0, 0, 0, 0, 0, 
	 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 2, 2, 2, 2, 2, 2,
	 0, 0, 2, 6, 6, 6, 6, 6,
	 0, 0, 2, 6, 6, 6, 6, 6,
	 0, 0, 2, 2, 2, 2, 2, 2,
	 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0,
	 
	 0, 0, 0, 0, 0, 0, 0, 0, 
	 0, 0, 0, 0, 0, 0, 0, 0,
	 2, 2, 2, 2, 2, 2, 2, 2,
	 6, 6, 6, 6, 6, 6, 6, 6,
	 6, 6, 6, 6, 6, 6, 6, 6,
	 2, 2, 2, 2, 2, 2, 2, 2,
	 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0,
	 
	 0, 0, 0, 0, 0, 0, 0, 0, 
	 0, 0, 0, 0, 0, 0, 0, 0,
	 2, 2, 2, 2, 2, 2, 2, 2,
	 6, 6, 6, 6, 6, 6, 6, 6,
	 6, 6, 6, 6, 6, 6, 6, 6,
	 2, 2, 2, 2, 2, 2, 2, 2,
	 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0,
	 
	 7, 7, 0, 0, 0, 0, 0, 0, 
	 2, 7, 7, 7, 7, 7, 7, 0,
	 2, 2, 2, 2, 2, 2, 7, 0,
	 6, 6, 6, 6, 6, 2, 7, 0,
	 6, 6, 6, 6, 6, 2, 7, 0,
	 2, 2, 2, 2, 2, 2, 7, 0,
	 2, 7, 7, 7, 7, 7, 7, 0,
	 7, 7, 0, 0, 0, 0, 0, 0};
	 
	 unsigned int laserPickupDraw[64] = 
	 
	{0, 0, 0, 0, 0, 0, 0, 0, 
	 0, 0, 8, 8, 8, 8, 0, 0,
	 0, 9, 6, 2, 2, 6, 9, 0,
	 0, 9, 2, 2, 6, 2, 9, 0,
	 0, 9, 6, 2, 2, 6, 9, 0,
	 0, 9, 2, 2, 6, 2, 9, 0,
	 0, 9, 6, 2, 6, 6, 9, 0,
	 0, 0, 8, 8, 8, 8, 0, 0};
	  
unsigned int heart[64] = 
	{0, 0, 0, 0, 0, 0, 0, 0,
	 0, 5, 5, 0, 0, 5, 5, 0,
	 5, 3, 3, 3, 3, 3, 3, 5,
	 5, 3, 3, 3, 3, 3, 3, 5,
	 0, 5, 3, 3, 3, 3, 5, 0,
	 0, 0, 5, 3, 3, 5, 0, 0,
	 0, 0, 0, 5, 5, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0};
	 
unsigned int text[] =
	{
	// 'L'
	0, 0, 15, 15, 0, 0, 0, 0,
	0, 0, 15, 15, 0, 0, 0, 0,
	0, 0, 15, 15, 0, 0, 0, 0,
	0, 0, 15, 15, 0, 0, 0, 0,
	0, 0, 15, 15, 0, 0, 0, 0,
	0, 0, 15, 15, 0, 0, 0, 0,
	0, 0, 15, 15, 15, 15, 0, 0,
	0, 0, 15, 15, 15, 15, 0, 0,
	
	//'i'
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 15, 15, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 15, 15, 15, 0, 0, 0, 0,
	0, 0, 15, 15, 0, 0, 0, 0,
	0, 0, 15, 15, 0, 0, 0, 0, 
	0, 0, 15, 15, 0, 0, 0, 0,
	0, 15, 15, 15, 15, 0, 0, 0,
	
	//'v'
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 15, 15, 0, 15, 15, 0, 0, 
	0, 15, 15, 0, 15, 15, 0, 0, 
	0, 15, 15, 0, 15, 15, 0, 0, 
	0, 15, 15, 0, 15, 15, 0, 0, 
	0, 0, 15, 15, 15, 0, 0, 0, 
	0, 0, 0, 15, 0, 0, 0, 0, 
	
	//'e'
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 15, 15, 15, 15, 0, 0, 0,
	15, 15, 0, 15, 15, 15, 0, 0, 
	15, 15, 15, 15, 15, 15, 0, 0, 
	15, 15, 0, 0, 0, 0, 0, 0, 
	15, 15, 0, 0, 0, 15, 0, 0,
	0, 15, 15, 15, 15, 0, 0, 0,
	
	//'s'
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 15, 15, 15, 15, 15, 0, 0,
	15, 15, 0, 0, 0, 0, 0, 0,
	15, 15, 15, 15, 15, 0, 0, 0, 
	0, 15, 15, 15, 15, 15, 0, 0, 
	0, 0, 0, 0, 15, 15, 0, 0, 
	15, 15, 15, 15, 15, 0, 0, 0,
	
	//'G'
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 15, 15, 15, 0, 0, 0,
	0, 15, 0, 0, 0, 15, 0, 0,
	0, 15, 0, 0, 0, 0, 0, 0,
	0, 15, 0, 0, 0, 0, 0, 0, 
	0, 15, 0, 0, 15, 15, 0, 0, 
	0, 15, 0, 0, 0, 15, 0, 0, 
	0, 0, 15, 15, 15, 0, 0, 0,
	
	//'A'
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 15, 15, 15, 0, 0, 0,
	0, 15, 0, 0, 0, 15, 0, 0,
	0, 15, 0, 0, 0, 15, 0, 0,
	0, 15, 15, 15, 15, 15, 0, 0, 
	0, 15, 0, 0, 0, 15, 0, 0, 
	0, 15, 0, 0, 0, 15, 0, 0, 
	0, 15, 0, 0, 0, 15, 0, 0,
	
	//'M'
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 15, 0, 0, 0, 15, 0, 0,
	0, 15, 15, 0, 15, 15, 0, 0,
	0, 15, 0, 15, 0, 15, 0, 0,
	0, 15, 0, 15, 0, 15, 0, 0, 
	0, 15, 0, 0, 0, 15, 0, 0, 
	0, 15, 0, 0, 0, 15, 0, 0, 
	0, 15, 0, 0, 0, 15, 0, 0,
	
	//'O'
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 15, 15, 15, 0, 0, 0,
	0, 15, 0, 0, 0, 15, 0, 0,
	0, 15, 0, 0, 0, 15, 0, 0,
	0, 15, 0, 0, 0, 15, 0, 0, 
	0, 15, 0, 0, 0, 15, 0, 0, 
	0, 15, 0, 0, 0, 15, 0, 0, 
	0, 0, 15, 15, 15, 0, 0, 0,
	
	//'V'
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 15, 0, 0, 0, 15, 0, 0,
	0, 15, 0, 0, 0, 15, 0, 0,
	0, 15, 0, 0, 0, 15, 0, 0,
	0, 15, 0, 0, 0, 15, 0, 0, 
	0, 0, 15, 0, 15, 0, 0, 0, 
	0, 0, 15, 0, 15, 0, 0, 0, 
	0, 0, 0, 15, 0, 0, 0, 0,
	
	//'R'
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 15, 15, 15, 15, 0, 0, 0,
	0, 15, 0, 0, 0, 15, 0, 0,
	0, 15, 0, 0, 0, 15, 0, 0,
	0, 15, 15, 15, 15, 0, 0, 0, 
	0, 15, 0, 15, 0, 0, 0, 0, 
	0, 15, 0, 0, 15, 0, 0, 0, 
	0, 15, 0, 0, 0, 15, 0, 0,
	
	//'P'
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 15, 15, 15, 15, 0, 0, 0,
	0, 15, 0, 0, 0, 15, 0, 0,
	0, 15, 0, 0, 0, 15, 0, 0,
	0, 15, 15, 15, 15, 0, 0, 0, 
	0, 15, 0, 0, 0, 0, 0, 0, 
	0, 15, 0, 0, 0, 0, 0, 0, 
	0, 15, 0, 0, 0, 0, 0, 0,
	
	//'S'
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 15, 15, 15, 15, 0, 0,
	0, 15, 0, 0, 0, 15, 0, 0,
	0, 15, 0, 0, 0, 0, 0, 0,
	0, 0, 15, 15, 15, 0, 0, 0, 
	0, 0, 0, 0, 0, 15, 0, 0, 
	0, 0, 0, 0, 0, 15, 0, 0, 
	0, 15, 15, 15, 15, 0, 0, 0,
	
	//'T'
	0, 0, 0, 0, 0, 0, 0, 0,
	15, 15, 15, 15, 15, 15, 15, 0,
	0, 0, 0, 15, 0, 0, 0, 0,
	0, 0, 0, 15, 0, 0, 0, 0,
	0, 0, 0, 15, 0, 0, 0, 0, 
	0, 0, 0, 15, 0, 0, 0, 0, 
	0, 0, 0, 15, 0, 0, 0, 0, 
	0, 0, 0, 15, 0, 0, 0, 0,
	
	//'I'
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 15, 15, 15, 0, 0, 0,
	0, 0, 0, 15, 0, 0, 0, 0,
	0, 0, 0, 15, 0, 0, 0, 0,
	0, 0, 0, 15, 0, 0, 0, 0, 
	0, 0, 0, 15, 0, 0, 0, 0, 
	0, 0, 0, 15, 0, 0, 0, 0, 
	0, 0, 15, 15, 15, 0, 0, 0,
	
	//'E'
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 15, 15, 15, 15, 15, 0, 0,
	0, 15, 0, 0, 0, 0, 0, 0,
	0, 15, 0, 0, 0, 0, 0, 0,
	0, 15, 15, 15, 15, 15, 0, 0, 
	0, 15, 0, 0, 0, 0, 0, 0, 
	0, 15, 0, 0, 0, 0, 0, 0, 
	0, 15, 15, 15, 15, 15, 0, 0,
	
	//'U'
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 15, 0, 0, 0, 15, 0, 0,
	0, 15, 0, 0, 0, 15, 0, 0,
	0, 15, 0, 0, 0, 15, 0, 0,
	0, 15, 0, 0, 0, 15, 0, 0, 
	0, 15, 0, 0, 0, 15, 0, 0, 
	0, 15, 0, 0, 0, 15, 0, 0, 
	0, 0, 15, 15, 15, 0, 0, 0,
	
	//'C'
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 15, 15, 15, 0, 0, 0,
	0, 15, 0, 0, 0, 15, 0, 0,
	0, 15, 0, 0, 0, 0, 0, 0,
	0, 15, 0, 0, 0, 0, 0, 0, 
	0, 15, 0, 0, 0, 0, 0, 0, 
	0, 15, 0, 0, 0, 15, 0, 0, 
	0, 0, 15, 15, 15, 0, 0, 0,
	
	//'D'
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 15, 15, 15, 15, 0, 0, 0,
	0, 15, 0, 0, 0, 15, 0, 0,
	0, 15, 0, 0, 0, 15, 0, 0,
	0, 15, 0, 0, 0, 15, 0, 0,
	0, 15, 0, 0, 0, 15, 0, 0, 
	0, 15, 0, 0, 0, 15, 0, 0,
	0, 15, 15, 15, 15, 0, 0, 0};
	
	unsigned int numbers[] =
	{
	// '0' Zero
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 15, 15, 15, 0, 0, 0,
	0, 15, 0, 0, 0, 15, 0, 0,
	0, 15, 0, 0, 15, 15, 0, 0,
	0, 15, 0, 15, 0, 15, 0, 0,
	0, 15, 15, 0, 0, 15, 0, 0,
	0, 15, 0, 0, 0, 15, 0, 0,
	0, 0, 15, 15, 15, 0, 0, 0,
	
	// '1'
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 15, 0, 0, 0, 0,
	0, 0, 15, 15, 0, 0, 0, 0,
	0, 0, 0, 15, 0, 0, 0, 0,
	0, 0, 0, 15, 0, 0, 0, 0,
	0, 0, 0, 15, 0, 0, 0, 0,
	0, 0, 0, 15, 0, 0, 0, 0,
	0, 0, 15, 15, 15, 0, 0, 0,
	
	// '2'
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 15, 15, 15, 0, 0, 0,
	0, 15, 0, 0, 0, 15, 0, 0,
	0, 0, 0, 0, 0, 15, 0, 0,
	0, 0, 0, 0, 15, 0, 0, 0,
	0, 0, 0, 15, 0, 0, 0, 0,
	0, 0, 15, 0, 0, 0, 0, 0,
	0, 15, 15, 15, 15, 15, 0, 0,
	
	// '3'
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 15, 15, 15, 15, 15, 0, 0,
	0, 0, 0, 0, 15, 0, 0, 0,
	0, 0, 0, 15, 0, 0, 0, 0,
	0, 0, 0, 0, 15, 0, 0, 0,
	0, 0, 0, 0, 0, 15, 0, 0,
	0, 15, 0, 0, 0, 15, 0, 0,
	0, 0, 15, 15, 15, 0, 0, 0,
	
	// '4'
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 15, 0, 0, 0,
	0, 0, 0, 15, 15, 0, 0, 0,
	0, 0, 15, 0, 15, 0, 0, 0,
	0, 15, 0, 0, 15, 0, 0, 0,
	0, 15, 15, 15, 15, 15, 0, 0,
	0, 0, 0, 0, 15, 0, 0, 0,
	0, 0, 0, 0, 15, 0, 0, 0,
	
	// '5'
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 15, 15, 15, 15, 15, 0, 0,
	0, 15, 0, 0, 0, 0, 0, 0,
	0, 15, 15, 15, 15, 0, 0, 0,
	0, 0, 0, 0, 0, 15, 0, 0,
	0, 0, 0, 0, 0, 15, 0, 0,
	0, 15, 0, 0, 0, 15, 0, 0,
	0, 0, 15, 15, 15, 0, 0, 0,
	
	// '15'
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 15, 15, 0, 0, 0,
	0, 0, 15, 0, 0, 0, 0, 0,
	0, 15, 0, 0, 0, 0, 0, 0,
	0, 15, 15, 15, 15, 0, 0, 0,
	0, 15, 0, 0, 0, 15, 0, 0,
	0, 15, 0, 0, 0, 15, 0, 0,
	0, 0, 15, 15, 15, 0, 0, 0,
	
	// '7'
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 15, 15, 15, 15, 15, 0, 0,
	0, 0, 0, 0, 0, 15, 0, 0,
	0, 0, 0, 0, 15, 0, 0, 0,
	0, 0, 0, 15, 0, 0, 0, 0,
	0, 0, 15, 0, 0, 0, 0, 0,
	0, 0, 15, 0, 0, 0, 0, 0,
	0, 0, 15, 0, 0, 0, 0, 0,
	
	// '8'
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 15, 15, 15, 0, 0, 0,
	0, 15, 0, 0, 0, 15, 0, 0,
	0, 15, 0, 0, 0, 15, 0, 0,
	0, 0, 15, 15, 15, 0, 0, 0,
	0, 15, 0, 0, 0, 15, 0, 0,
	0, 15, 0, 0, 0, 15, 0, 0,
	0, 0, 15, 15, 15, 0, 0, 0,
	
	// '9'
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 15, 15, 15, 0, 0, 0,
	0, 15, 0, 0, 0, 15, 0, 0,
	0, 15, 0, 0, 0, 15, 0, 0,
	0, 0, 15, 15, 15, 15, 0, 0,
	0, 0, 0, 0, 0, 15, 0, 0,
	0, 0, 0, 0, 15, 0, 0, 0,
	0, 0, 15, 15, 0, 0, 0, 0};