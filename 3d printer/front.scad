// --------- SNAKEBOY - FRONT PANEL v5 (with male studs) ---------

$fn = 64;

// General dimensions
box_length = 152;
box_width  = 112;
box_depth  = 33;
wall_t = 3;
front_t = 3;
corner_r = 10;

// Matrix MAX7219 window
matrix_w = 132.15;
matrix_h = 32.15; // .15 to leave some space for the matrix to fit in/out
matrix_y = 14;

// Joystick (KY-023)
joy_d = 20; // hole diameter
joy_x = 30;
joy_y = box_width - 35;

// Cross buttons (4 buttons)
btn_d = 11.5; // approximate real diameter
btn_spacing = 8.5; // spacing between centers
btn_center_x = box_length - 40;
btn_center_y = box_width - 38;

// Buzzer
buzzer_d = 22;
buzzer_x = joy_x+30;
buzzer_y = joy_y + 18;

// Indicator LED
led_d = 5;
led_x = btn_center_x+20;
led_y = btn_center_y + 26;

// Screws
screw_d = 3.0;
screw_offset = 10;

// Cable hole parameters
cable_d = 6;   // cable diameter
cable_x = box_length*3/4;   // centered in X
cable_y = box_width+1;      // near the front edge
cable_z = box_depth/2;    // centered in height

// USB hole parameters
// Mini-B USB hole parameters
usb_d = 6;   // cable diameter
usb_x = box_length/6 +8;  // left side +8 for exact Mini-B position
usb_y = box_width+1;     // centered in Y
usb_z = box_depth-1;     // near the bottom

// ------- Male stud module -------
module male_stud(x, y, stud_d, stud_h, hole_d) {
    // Stud body (cylinder protruding from the front face)
    translate([x, y, front_t])
        cylinder(h = stud_h, d = stud_d);

    // Central hole in the stud (for screwing from the back)
    translate([x, y, front_t - 1])
        cylinder(h = stud_h + 3, d = hole_d);
}

// ------- Rounded box shape -------
module rounded_box(l,w,h,r){
    hull(){
        for(x=[r,l-r])
            for(y=[r,w-r])
                translate([x,y,0]) cylinder(h=h, r=r);
    }
}

// ------- Main body construction -------
difference(){

    // Complete front enclosure
    rounded_box(box_length, box_width, box_depth, corner_r);

    // Hollow out the interior
    translate([wall_t,wall_t,front_t])
        rounded_box(box_length-2*wall_t, box_width-2*wall_t, box_depth, corner_r-wall_t);

    // Matrix MAX7219 window
    translate([(box_length-matrix_w)/2,
               matrix_y,
               -1])
        cube([matrix_w,matrix_h,front_t+4]);

    // Joystick hole
    translate([joy_x, joy_y, -1])
        cylinder(h=front_t+4, d=joy_d);

    // 4 cross buttons
    // Top button (△)
    translate([btn_center_x, btn_center_y - btn_spacing, -1])
        cylinder(h=front_t+4, d=btn_d);
    // Bottom button (X)
    translate([btn_center_x, btn_center_y + btn_spacing, -1])
        cylinder(h=front_t+4, d=btn_d);
    // Left button (□)
    translate([btn_center_x - btn_spacing, btn_center_y, -1])
        cylinder(h=front_t+4, d=btn_d);
    // Right button (◯)
    translate([btn_center_x + btn_spacing, btn_center_y, -1])
        cylinder(h=front_t+4, d=btn_d);

    // LED
    translate([led_x, led_y-7, -1])
        cylinder(h=front_t+4, d=led_d);

    // Buzzer (simple round hole)
    translate([buzzer_x, buzzer_y, -1])
        cylinder(h=front_t+4, d=buzzer_d);
        
    // Cable hole on the right side
    translate([cable_x, cable_y, cable_z])
    rotate([90,0,0])   // orient cylinder towards the front
    cylinder(h=wall_t+2, d=cable_d);
        
    // Rectangular hole for Mini-B USB
    translate([usb_x, usb_y, usb_z])
    rotate([90,90,0])   // orient cube horizontally
     cube([wall_t+2, 8, 5]);   //cylinder(h=wall_t+2, d=usb_d);

    // L3
    // Small rectangular hole to the right of the joystick (for L3 / clearance)
    translate([joy_x + 8, joy_y-4, -1])
    cube([front_t + 2, 9, 5]);  // [thickness, Y=9, X=5]
    
    //

}

// Male studs in the front face for screwing from the back

male_stud(x = screw_offset, y = screw_offset, stud_d = 6, stud_h = 30, hole_d = 3.2);

male_stud(x = box_length - screw_offset, y = screw_offset, stud_d = 6, stud_h = 30, hole_d = 3.2);

male_stud(x = screw_offset, y = box_width - screw_offset, stud_d = 6, stud_h = 30, hole_d = 3.2);

male_stud(x = box_length - screw_offset, y = box_width - screw_offset, stud_d = 6, stud_h = 30, hole_d = 3.2);

/*
// ------- Internal anti-flex reinforcement ribs -------
for(x=[30:30:(box_length-30)])
    translate([x, wall_t, front_t])
        cube([2,box_width-2*wall_t,6]);
*/