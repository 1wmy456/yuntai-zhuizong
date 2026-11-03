module ov5640_capture_data(
    input           rst_n,

    input           cam_pclk,
    input           cam_vsync,
    input           cam_href,
    input   [7:0]   cam_data,
    output          cam_rst_n,
    output          cam_pwdn,

    output          vid_clk,
    output          vid_ce,
    output          vid_vsync,
    output          vid_active_video,
    output  [23:0]  vid_data
    );

parameter WAIT_FRAME = 10;

reg            rst_n_d0 ;
reg            rst_n_sync ;
reg            cam_vsync_d0;
reg            cam_vsync_d1;
reg            cam_href_d0;
reg            cam_href_d1;
reg            byte_flag_d0;

reg  [5:0]     frame_cnt;
reg            wait_done;
reg  [7:0]     cam_data_d0;
reg            byte_flag;
reg  [15:0]    rgb565_data;

wire           pos_vsync;
wire [23:0]    rgb888_data;

assign pos_vsync = ~cam_vsync_d1 & cam_vsync_d0;

assign cam_rst_n = 1'b1;

assign cam_pwdn = 1'b0;

assign vid_clk = cam_pclk;

assign rgb888_data = {rgb565_data[15:11],3'b0,rgb565_data[10:5],2'b0,rgb565_data[4:0],3'b0};
assign vid_data = wait_done ?  rgb888_data : 24'd0;

assign vid_vsync = wait_done ? cam_vsync_d1 : 1'd0;
assign vid_active_video = wait_done ? cam_href_d1 : 1'b0;

assign vid_ce = wait_done ? ((byte_flag_d0 & vid_active_video) || (!vid_active_video)) : 1'b0;

always @(posedge cam_pclk or negedge rst_n) begin
    if(!rst_n) begin
        rst_n_d0 <= 1'b0;
        rst_n_sync <= 1'b0;
    end
    else begin
        rst_n_d0 <= 1'b1;
        rst_n_sync <= rst_n_d0;
    end
end

always @(posedge cam_pclk or negedge rst_n_sync) begin
    if(!rst_n_sync) begin
        cam_vsync_d0 <= 1'b0;
        cam_vsync_d1 <= 1'b0;
        cam_href_d0 <= 1'b0;
        cam_href_d1 <= 1'b0;
    end
    else begin
        cam_vsync_d0 <= cam_vsync;
        cam_vsync_d1 <= cam_vsync_d0;
        cam_href_d0 <= cam_href;
        cam_href_d1 <= cam_href_d0;
    end
end

always @(posedge cam_pclk or negedge rst_n_sync) begin
    if(!rst_n_sync)
        frame_cnt <= 4'd0;
    else if(pos_vsync && (frame_cnt < WAIT_FRAME))
        frame_cnt <= frame_cnt + 4'd1;
end

always @(posedge cam_pclk or negedge rst_n_sync) begin
    if(!rst_n_sync)
        wait_done <= 1'b0;
    else if(pos_vsync && (frame_cnt == WAIT_FRAME))
        wait_done <= 1'b1;
end

always @(posedge cam_pclk or negedge rst_n_sync) begin
    if(!rst_n_sync) begin
        cam_data_d0 <= 8'd0;
        rgb565_data <= 16'd0;
        byte_flag <= 1'b0;
    end
    else if(cam_href) begin
        byte_flag <= ~byte_flag;
        cam_data_d0 <= cam_data;
        if(byte_flag)
            rgb565_data <= {cam_data_d0,cam_data};
    end
    else begin
        byte_flag <= 1'b0;
        cam_data_d0 <= 8'd0;
        rgb565_data <= 16'd0;
    end
end

always @(posedge cam_pclk or negedge rst_n_sync) begin
    if(!rst_n_sync)
        byte_flag_d0 <= 1'b0;
    else
        byte_flag_d0 <= byte_flag;
end

endmodule
