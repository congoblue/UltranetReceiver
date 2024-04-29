// Definition size of housing
// **************************

       // Those settings have to be identical to the cover !!!
width_x      = 130;          // Width of the housing (outer dimension) 
debth_y      = 85;           // Debth of the housing (outer dimension)
wall         = 1.8;          // Wall thickness of the box
cornerradius = 4.0;          // Radius of the corners
                             //   This value also defines the posts for stability and
                             //   for the press-in nuts!

       // Those settings are more or less independent from the cover
height_z     = 4
;           // Heigth of the lower part. Total height is this value plus
                             //   the height of the cover
plate        = 1.4;          // Thickness of the bottom plain

       //Definition size of press-in nuts
nutdiameter  = 4.1;          // Hole diameter for thermal press-in nut
nutlength    = 5.8;          // Depth of the nut hole
screwdia = 3.2;
screwhead = 6;


//Definition of circle angular resolution
resol        = 36;



difference () {

// Construction of housing
   union () {
   // Definition of main body
      difference () {
         union () {
            cube ( [width_x, debth_y - (2* cornerradius), height_z], center = true );
            cube ( [width_x - (2* cornerradius), debth_y, height_z], center = true );
         };
         translate ( [0,0, plate / 2] ){
            cube ( [width_x - (2* wall), debth_y- (2* wall), height_z - plate], center = true ); 
         };
      };

      // Construction of four corner cylinders including nut holes
      // 1st quadrant
      translate ( [(width_x / 2) - cornerradius, (debth_y / 2) - cornerradius, 0 ] ) {
         difference () {
            cylinder (h=height_z, r=cornerradius, center = true, $fn = resol);
            translate ( [ 0,0,(height_z / 2)-(nutlength / 2) ] ) {
               cylinder (h = nutlength, r = screwdia / 2, center = true, $fn = resol);
            };
         };
      };

      // 2nd quadrant
      translate ( [-(width_x / 2) + cornerradius, (debth_y / 2) - cornerradius, 0] ) {
         difference () {
            cylinder (h=height_z, r=cornerradius, center = true, $fn = resol);
            translate ( [ 0,0,(height_z / 2)-(nutlength / 2) ] ) {
               cylinder (h = nutlength, r = screwdia / 2, center = true, $fn = resol);
            };
         };
      };

      // 4th quadrant
      translate ( [(width_x / 2) - cornerradius, -(debth_y / 2) + cornerradius, 0] ) {
         difference () {
            cylinder (h=height_z, r=cornerradius, center = true, $fn = resol);
            translate ( [ 0,0,(height_z / 2)-(nutlength / 2) ] ) {
               cylinder (h = nutlength, r = screwdia / 2, center = true, $fn = resol);
            };
         };
      };

      // 3rd quadrant
      translate ( [-(width_x / 2) + cornerradius, -(debth_y / 2) + cornerradius, 0] ) {
         difference () {
            cylinder (h=height_z, r=cornerradius, center = true, $fn = resol);
            translate ( [ 0,0,(height_z / 2)-(nutlength / 2) ] ) {
               cylinder (h = nutlength, r = screwdia / 2, center = true, $fn = resol);
            };
         };
      }; 
      
      //spacer for rot encoder
      translate ([36,29.7,0.4]) rotate ([0,0,90]) cylinder (h=2, r=12/2, center = true, $fn = resol);
      
      //side flanges
      translate ([0,39.8,2]) cube ([width_x-20,1.7,4], center=true);
      translate ([0,-39.8,2]) cube ([width_x-20,1.7,4], center=true);
      translate ([62.4,0,2]) cube ([1.7,debth_y-16,4], center=true);
      translate ([-62.4,0,2]) cube ([1.7,debth_y-16,4], center=true);
      
      //mount for switch cap assy
      translate ([0,36.8,0]) cube ([44,1.3,1.3], center=true);
      
   };

// Space for the construction of holes, breakouts, ... 
   translate ([0,0,0]) cube ( [50,39,8], center = true ); //disp
   translate ([36,29.7,0]) rotate ([0,0,90]) cylinder (h=8, r=7/2, center = true, $fn = resol);

   translate ([0,29.3,0]) cube ( [50,12,8], center = true ); //buttons
   
   //recess for screws
   translate ( [(width_x / 2) - cornerradius, (debth_y / 2) - cornerradius, -3.4 ] ) cylinder (h = nutlength, r = screwhead / 2, center = true, $fn = resol);
   translate ( [-(width_x / 2) + cornerradius, (debth_y / 2) - cornerradius, -3.4 ] ) cylinder (h = nutlength, r = screwhead / 2, center = true, $fn = resol);
   translate ( [(width_x / 2) - cornerradius, -(debth_y / 2) + cornerradius, -3.4 ] ) cylinder (h = nutlength, r = screwhead / 2, center = true, $fn = resol);
   translate ( [-(width_x / 2) + cornerradius, -(debth_y / 2) + cornerradius, -3.4 ] ) cylinder (h = nutlength, r = screwhead / 2, center = true, $fn = resol);


};


//pcb pillars
difference () {
    union () {
        translate ([-49.5,36.6,5-height_z/2+plate]) rotate ([0,0,90]) cylinder (h=9, r=6/2, center = true, $fn = resol);
        translate ([-49.5,-37.7,5-height_z/2+plate]) rotate ([0,0,90]) cylinder (h=9, r=6/2, center = true, $fn = resol);
        translate ([54.9,36.6,5-height_z/2+plate]) rotate ([0,0,90]) cylinder (h=9, r=6/2, center = true, $fn = resol);
        translate ([54.9,-37.7,5-height_z/2+plate]) rotate ([0,0,90]) cylinder (h=9, r=6/2, center = true, $fn = resol);
    };
    translate ([-49.5,36.6,5-height_z/2+plate]) rotate ([0,0,90]) cylinder (h=9, r=nutdiameter/2, center = true, $fn = resol);
    translate ([-49.5,-37.7,5-height_z/2+plate]) rotate ([0,0,90]) cylinder (h=9, r=nutdiameter/2, center = true, $fn = resol);
    translate ([54.9,36.6,5-height_z/2+plate]) rotate ([0,0,90]) cylinder (h=9, r=nutdiameter/2, center = true, $fn = resol);
    translate ([54.9,-37.7,5-height_z/2+plate]) rotate ([0,0,90]) cylinder (h=9, r=nutdiameter/2, center = true, $fn = resol);
    
};
