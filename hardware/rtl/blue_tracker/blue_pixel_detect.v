module blue_pixel_detect #(
    parameter integer R_BYTE = 2, G_BYTE = 1, B_BYTE = 0,
    parameter integer BLUE_MIN = 80, BR_DELTA = 35, BG_DELTA = 20,
    parameter integer MIN_B_R_RATIO = 0
)(input [23:0] pixel, output is_blue);
    wire [7:0] r = pixel[R_BYTE*8 +: 8];
    wire [7:0] g = pixel[G_BYTE*8 +: 8];
    wire [7:0] b = pixel[B_BYTE*8 +: 8];

    assign is_blue = ({24'd0,b} >= BLUE_MIN) &&
                     ({24'd0,b} > ({24'd0,r} + BR_DELTA)) &&
                     ({24'd0,b} > ({24'd0,g} + BG_DELTA)) &&
                     (MIN_B_R_RATIO == 0 || {24'd0,b} > {24'd0,r} * MIN_B_R_RATIO);
endmodule
