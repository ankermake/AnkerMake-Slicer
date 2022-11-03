#version 330 core


layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 ankerInfo_1;
layout (location = 2) in vec4 ankerInfo_2;
layout (location = 3) in vec3 ankerInfo_3;
layout (location = 4) in vec3 ankerInfo_4;
layout (location = 5) in vec3 ankerInfo_5;

// delta-extruder freedrate w h
// mm3_per_mm fan_speed layer_duration time
// temperature motor_process.accelerate_until  motor_process.cruise
// motor_process.decelerate_after motor_process.entry  motor_process.exit
// type extrusion_role  g1_line_id


out ankerInfo{
    vec4 ak_info_1;
    vec4 ak_info_2;
    vec3 ak_info_3;
    vec3 ak_info_4;
    vec3 ak_info_5;
} vs_out;


void main()
{
    gl_Position = vec4(aPos,1.0);
    vs_out.ak_info_1 = ankerInfo_1;
    vs_out.ak_info_2 = ankerInfo_2;
    vs_out.ak_info_3 = ankerInfo_3;
    vs_out.ak_info_4 = ankerInfo_4;
    vs_out.ak_info_5 = ankerInfo_5;
}
