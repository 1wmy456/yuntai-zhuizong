module rgb2lcd #(

    parameter    VID_DATA_WIDTH = 24
)(

    input  [VID_DATA_WIDTH-1:0]   rgb_data ,
    input                         rgb_vde  ,
    input                         rgb_hsync,
    input                         rgb_vsync,

    input                         pixel_clk,

    output                        lcd_pclk ,
    output                        lcd_rst  ,
    output                        lcd_hs   ,
    output                        lcd_vs   ,
    output                        lcd_de   ,
    output                        lcd_bl   ,

    input  [2:0]                  lcd_id_i,
    input  [2:0]                  lcd_id_t,
    output [2:0]                  lcd_id_o,

    input  [VID_DATA_WIDTH-1:0]   lcd_rgb_i,
    output [VID_DATA_WIDTH-1:0]   lcd_rgb_o,
    output [VID_DATA_WIDTH-1:0]   lcd_rgb_t
);

assign lcd_pclk = pixel_clk;
assign lcd_de = rgb_vde  ;
assign lcd_hs = rgb_hsync;
assign lcd_vs = rgb_vsync;
assign lcd_bl = 1'b1;
assign lcd_rst = 1'b1;

assign lcd_id_o[0] = (lcd_id_t[0]==1'b1) ? lcd_rgb_i[23] : 1'bz;
assign lcd_id_o[1] = (lcd_id_t[1]==1'b1) ? lcd_rgb_i[15] : 1'bz;
assign lcd_id_o[2] = (lcd_id_t[2]==1'b1) ? lcd_rgb_i[7] : 1'bz;

assign lcd_rgb_o  = (lcd_id_t[0]==1'b1) ? {VID_DATA_WIDTH{1'bz}} :
                                           rgb_data[VID_DATA_WIDTH-1:0];
assign lcd_rgb_t  = {VID_DATA_WIDTH{lcd_id_t[0]}};

endmodule
