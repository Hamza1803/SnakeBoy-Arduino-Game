$fn = 64;

// General dimensions
box_length = 152;
box_width  = 112;
front_t = 3;
corner_r = 10;
screw_d = 3.15;
screw_offset = 10;

// Matrix window
matrix_w = 125;
matrix_h = 32;
matrix_y = 12;


// --------- ROUNDED BACK PLATE ----------
module back_plate() {
    hull(){
        for(x=[corner_r, box_length-corner_r])
            for(y=[corner_r, box_width-corner_r])
                translate([x, y, 0]) cylinder(h = front_t, r = corner_r);
    }
}


// -------- SAVEMAKER HOLDER --------
module savemaker_bottom_holder() {
    size = 54.6;
    wall = 1.8;
    height = 8;

    translate([5-2-60, -2, 0])
        cube([wall, size, height]);

    translate([size-60 - wall+6.15, -2, 0])  // .15 to tighten the Arduino
        cube([wall, size, height]);

    translate([3.6-60, -2, 0])
        cube([size+2, wall, height]);
}



// ================= FULL BACK PLATE ==================
union() {

    // --- TOP WITH CUTOUTS ---
    difference() {
        back_plate();

        // 4 screw holes
        for(x = [screw_offset, box_length - screw_offset])
            for(y = [screw_offset, box_width - screw_offset])
                translate([x, y, -1])
                    cylinder(h = front_t + 2, d = screw_d);

        // Text on the back
        translate([box_length/2, box_width/2,2])
            rotate([0,0,180])
            linear_extrude(height = 1.75)
                text("SNAKE BOY", size = 15, font = "Pricedown:style=Bold",
                     halign = "center", valign = "center");
    }


    // ===================
    //   3 X FOR MATRIX (FRONT SIDE)
    // ===================

    spacing = matrix_w / 3;

    

    // --- SAVEMAKER HOLDER ---
    translate([
        box_length/4 + 37,
        box_width - 57,
        front_t-10
    ])
        savemaker_bottom_holder();
}