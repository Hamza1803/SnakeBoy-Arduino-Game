// --------- XBOX A B X Y BUTTONS - STANDALONE (WITH RETENTION COLLAR) --------
// Designed to be printed separately or integrated into a panel
// Stay inside the box thanks to an internal collar

$fn = 64;

// === Adjustable parameters ===
btn_spacing = 8.5;    // distance between the center and each button
button_head_d = 11.20;    // diameter of the visible head (must be > btn_d)
retainer_d = 11.95;       // collar diameter (must be > btn_d, < space between buttons)
retainer_height = 1.2; // thin height of the collar (rests on the inside of the panel)
button_height = 6;     // total height of the visible head
stem_d = 6;            // stem diameter (for microswitch)
stem_height = 3;       // stem height
label_size = 4;
label_depth = 0.6;

// ------- Module: one button with collar and letter -------
module xbox_button(letter, dx, dy) {
    
    translate([dx, dy, retainer_height])
        cylinder(h = button_height, d = button_head_d);

    
    translate([dx, dy, 0])
        cylinder(h = retainer_height, d = retainer_d);

    
    translate([dx, dy, 0])
        cylinder(h = stem_height, d = stem_d);

    /
    translate([dx, dy, retainer_height + button_height - label_depth])
        linear_extrude(height = label_depth)
            text(letter, size = label_size, font = "Arial:style=Bold", halign="center", valign="center");
}

// Assembly of the 4 buttons 
union() {
    xbox_button("X", 0, -btn_spacing);   // Top
    xbox_button("A", 0, +btn_spacing);   // Bottom
    xbox_button("Y", -btn_spacing, 0);   // Left
    xbox_button("B", +btn_spacing, 0);   // Right
}