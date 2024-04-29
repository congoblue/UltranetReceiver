difference() {

   union () {
   translate ([0,0,-1.5+1.4]) cube ( [49,11.5,3], center = true ); //buttons below panel

   translate ([0,0,-1.5+3]) cube ( [49,11.5,3], center = true ); //buttons above panel

   translate ([0,-7.6,-1]) cube ( [49,3,2], center = true ); //buttons
   translate ([0,-6.5,-0.4]) cube ( [49,3,0.4], center = true ); //buttons

   translate ([0,6,-0.8]) cube ( [49,2,1.5], center = true ); //buttons


   //translate ([0,-8.5,0.7]) cube ( [49,4,1.4], center = true ); //buttons

   //translate ([0,-6.5,-0.4]) cube ( [49,4,3], center = true ); //buttons


   };

   translate ([0,1.3,0]) cube ( [0.5,15,9], center = true ); //buttons
   translate ([12.25,1.3,0]) cube ( [0.5,15,9], center = true ); //buttons
   translate ([-12.25,1.3,0]) cube ( [0.5,15,9], center = true ); //buttons


   translate ([0,-7.6,-0.75]) cube ( [45,1.5,1.5], center = true ); //buttons

   
};