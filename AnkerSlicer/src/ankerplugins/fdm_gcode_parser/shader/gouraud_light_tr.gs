#version 330 core


layout (lines) in;
layout (triangle_strip, max_vertices = 40) out;

in ankerInfo{
    vec4 ak_info_1;     // delta-extruder freedrate w h
    vec4 ak_info_2;     // mm3_per_mm fan_speed layer_duration time
    vec3 ak_info_3;     // temperature motor_process.accelerate_until  motor_process.cruise
    vec3 ak_info_4;     // motor_process.decelerate_after motor_process.entry  motor_process.exit
    vec3 ak_info_5;     // type extrusion_role  g1_line_id
} anker_info[];

uniform mat4 mv_matrix;
uniform mat4 p_matrix;
uniform vec2 colorRange;
uniform int renderType;// 0 linetype 1 speed 2 T-render
uniform vec4 uniform_color;


out vec3 normal;
out vec4 aPos;
out vec3 mColor;

vec3 hsl2rgb( in vec3 c)
{
    vec3 rgb = clamp( abs(mod(c.x*6.0+vec3(0.0,4.0,2.0),6.0)-3.0)-1.0, 0.0, 1.0 );
    return c.z + c.y * (rgb-0.5)*(1.0-abs(2.0*c.z-1.0));
}



void main() {
     vec3 dir = normalize(gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz);
     vec3 right = normalize(vec3(dir.y, -dir.x,0.0));
     vec3 left = -right;
     vec3 up = cross(right,dir);
     vec3 down = -up;
     float half_width = 0.5f * anker_info[1].ak_info_1.z;
     float half_height = 0.5f * anker_info[1].ak_info_1.w;
     vec3  prev_pos =  gl_in[0].gl_Position.xyz - half_height * up;
     vec3  curr_pos =  gl_in[1].gl_Position.xyz - half_height * up;
     vec3 d_up = half_height * up;
     vec3 d_down = -half_height * up;
     vec3 d_right = half_width * right;
     vec3 d_left = -half_width * right;
     vec3 d_dir = half_width * dir;
     float rangeLen = colorRange.y - colorRange.x;
     vec3 lastColor;
     vec3 curColor;
     vec3 acc_pos;
     vec3 dece_pos;
     vec3 accColor;
     vec3 deceColor;

     switch (renderType) {
     case 1:
         float currFreedColor = (anker_info[1].ak_info_1.y - colorRange.x) / rangeLen;
         float lastFreedColor = (anker_info[0].ak_info_1.y - colorRange.x) / rangeLen;
         curColor = uniform_color.a > 0.001 ? uniform_color.rgb : hsl2rgb(vec3(currFreedColor * 0.5,0.88,0.68));
         lastColor = curColor;
         break;
     case 2:
        acc_pos = prev_pos + anker_info[1].ak_info_3.y * dir;
        dece_pos = prev_pos + anker_info[1].ak_info_4.x * dir;
        float last_t_f = (anker_info[1].ak_info_4.y - colorRange.x) / rangeLen; // A
        float acc_t_f = (anker_info[1].ak_info_3.z - colorRange.x) / rangeLen;  // B
        float dece_t_f = (anker_info[1].ak_info_3.z - colorRange.x) / rangeLen; // C
        float cur_t_f = (anker_info[1].ak_info_4.z - colorRange.x) / rangeLen;  // D
        lastColor = uniform_color.a > 0.001 ? uniform_color.rgb : hsl2rgb(vec3(last_t_f * 0.5,0.88,0.68));
        accColor = uniform_color.a > 0.001 ? uniform_color.rgb : hsl2rgb(vec3(acc_t_f * 0.5,0.88,0.68));
        deceColor = uniform_color.a > 0.001 ? uniform_color.rgb : hsl2rgb(vec3(dece_t_f * 0.5,0.88,0.68));
        curColor = uniform_color.a > 0.001 ? uniform_color.rgb : hsl2rgb(vec3(cur_t_f * 0.5,0.88,0.68));
        break;
     case 3:
        float curFlow = anker_info[1].ak_info_1.y * anker_info[1].ak_info_2.x;
        float lastFlow = anker_info[0].ak_info_1.y * anker_info[0].ak_info_2.x;
        float currFlowColor = (colorRange.y - curFlow) / rangeLen;
        float lastFlowColor = (colorRange.y - lastFlow) / rangeLen;
        curColor =  uniform_color.a > 0.001 ? uniform_color.rgb : hsl2rgb(vec3(currFlowColor * 0.65,0.88,0.55));
        lastColor = curColor;
        break;
     default:
        curColor = uniform_color.rgb;
        lastColor = curColor;
         break;
     }


    //8
    gl_Position = p_matrix*mv_matrix*vec4(prev_pos - d_dir,gl_in[0].gl_Position.w) ;
    normal = -dir;
    aPos = vec4(prev_pos - d_dir,gl_in[0].gl_Position.w);
    mColor = lastColor;
    EmitVertex();
    //0
    gl_Position = p_matrix*mv_matrix*vec4(prev_pos + d_up,gl_in[0].gl_Position.w) ;
    normal = up;
    aPos = vec4(prev_pos + d_up,gl_in[0].gl_Position.w);
    mColor = lastColor;
    EmitVertex();
    //1
    gl_Position = p_matrix*mv_matrix*vec4(prev_pos + d_right,gl_in[0].gl_Position.w);
    normal = right;
    aPos = vec4(prev_pos + d_right,gl_in[0].gl_Position.w);
    mColor = lastColor;
    EmitVertex();
    if(renderType == 2)
    {
    //10
    gl_Position = p_matrix*mv_matrix*vec4(acc_pos + d_up,gl_in[0].gl_Position.w) ;
    normal = up;
    aPos = vec4(acc_pos + d_up,gl_in[0].gl_Position.w);
    mColor = accColor;
    EmitVertex();
    //11
    gl_Position = p_matrix*mv_matrix*vec4(acc_pos + d_right,gl_in[0].gl_Position.w);
    normal = right;
    aPos = vec4(acc_pos + d_right,gl_in[0].gl_Position.w);
    mColor = accColor;
    EmitVertex();
    //14
    gl_Position = p_matrix*mv_matrix*vec4(dece_pos + d_up,gl_in[0].gl_Position.w) ;
    normal = up;
    aPos = vec4(dece_pos + d_up,gl_in[0].gl_Position.w);
    mColor = deceColor;
    EmitVertex();
    //15
    gl_Position = p_matrix*mv_matrix*vec4(dece_pos + d_right,gl_in[0].gl_Position.w);
    normal = right;
    aPos = vec4(dece_pos + d_right,gl_in[0].gl_Position.w);
    mColor = deceColor;
    EmitVertex();
    }
    //4
    gl_Position = p_matrix*mv_matrix*vec4(curr_pos + d_up,gl_in[1].gl_Position.w);
    normal = up;
    aPos = vec4(curr_pos + d_up,gl_in[1].gl_Position.w);
    mColor = curColor;
    EmitVertex();
    //5
    gl_Position = p_matrix*mv_matrix*vec4(curr_pos + d_right,gl_in[1].gl_Position.w);
    normal = right;
    aPos = vec4(curr_pos + d_right,gl_in[1].gl_Position.w);
    mColor = curColor;
    EmitVertex();
    //9
    gl_Position = p_matrix*mv_matrix*vec4(curr_pos + d_dir,gl_in[0].gl_Position.w) ;
    normal = dir;
    aPos = vec4(curr_pos + d_dir,gl_in[0].gl_Position.w);
    mColor = curColor;
    EmitVertex();
    EndPrimitive();



    //8
    gl_Position = p_matrix*mv_matrix*vec4(prev_pos - d_dir,gl_in[0].gl_Position.w) ;
    normal = -dir;
    aPos = vec4(prev_pos - d_dir,gl_in[0].gl_Position.w);
    mColor = lastColor;
    EmitVertex();
    //1
    gl_Position = p_matrix*mv_matrix*vec4(prev_pos + d_right,gl_in[0].gl_Position.w);
    normal = right;
    aPos = vec4(prev_pos + d_right,gl_in[0].gl_Position.w);
    mColor = lastColor;
    EmitVertex();
    //2
    gl_Position = p_matrix*mv_matrix*vec4(prev_pos + d_down,gl_in[0].gl_Position.w);
    normal = down;
    aPos = vec4(prev_pos + d_down,gl_in[0].gl_Position.w);
    mColor = lastColor;
    EmitVertex();
    if(renderType == 2)
    {
    //11
    gl_Position = p_matrix*mv_matrix*vec4(acc_pos + d_right,gl_in[0].gl_Position.w);
    normal = right;
    aPos = vec4(acc_pos + d_right,gl_in[0].gl_Position.w);
    mColor = accColor;
    EmitVertex();
    //12
    gl_Position = p_matrix*mv_matrix*vec4(acc_pos + d_down,gl_in[0].gl_Position.w);
    normal = down;
    aPos = vec4(acc_pos + d_down,gl_in[0].gl_Position.w);
    mColor = accColor;
    EmitVertex();
    //15
    gl_Position = p_matrix*mv_matrix*vec4(dece_pos + d_right,gl_in[0].gl_Position.w);
    normal = right;
    aPos = vec4(dece_pos + d_right,gl_in[0].gl_Position.w);
    mColor = mColor = deceColor;
    EmitVertex();
    //16
    gl_Position = p_matrix*mv_matrix*vec4(dece_pos + d_down,gl_in[0].gl_Position.w);
    normal = down;
    aPos = vec4(dece_pos + d_down,gl_in[0].gl_Position.w);
    mColor = deceColor;
    EmitVertex();
    }
    //5
    gl_Position = p_matrix*mv_matrix*vec4(curr_pos + d_right,gl_in[1].gl_Position.w);
    normal = right;
    aPos = vec4(curr_pos + d_right,gl_in[1].gl_Position.w);
    mColor = curColor;
    EmitVertex();
    //6
    gl_Position = p_matrix*mv_matrix*vec4(curr_pos + d_down,gl_in[1].gl_Position.w);
    normal = down;
    aPos = vec4(curr_pos + d_down,gl_in[1].gl_Position.w);
    mColor = curColor;
    EmitVertex();
    //9
    gl_Position = p_matrix*mv_matrix*vec4(curr_pos + d_dir,gl_in[0].gl_Position.w) ;
    normal = dir;
    aPos = vec4(curr_pos + d_dir,gl_in[0].gl_Position.w);
    mColor = curColor;
    EmitVertex();
    EndPrimitive();



    //8
    gl_Position = p_matrix*mv_matrix*vec4(prev_pos - d_dir,gl_in[0].gl_Position.w) ;
    normal = -dir;
    aPos = vec4(prev_pos - d_dir,gl_in[0].gl_Position.w);
    mColor = lastColor;
    EmitVertex();
    //2
    gl_Position = p_matrix*mv_matrix*vec4(prev_pos + d_down,gl_in[0].gl_Position.w);
    normal = down;
    aPos = vec4(prev_pos + d_down,gl_in[0].gl_Position.w);
    mColor = lastColor;
    EmitVertex();
    //3
    gl_Position = p_matrix*mv_matrix*vec4(prev_pos + d_left,gl_in[0].gl_Position.w);
    normal = left;
    aPos = vec4(prev_pos + d_left,gl_in[0].gl_Position.w);
    mColor = lastColor;
    EmitVertex();
    if(renderType == 2)
    {
    //12
    gl_Position = p_matrix*mv_matrix*vec4(acc_pos + d_down,gl_in[0].gl_Position.w);
    normal = down;
    aPos = vec4(acc_pos + d_down,gl_in[0].gl_Position.w);
    mColor = accColor;
    //13
    gl_Position = p_matrix*mv_matrix*vec4(acc_pos + d_left,gl_in[0].gl_Position.w);
    normal = left;
    aPos = vec4(acc_pos + d_left,gl_in[0].gl_Position.w);
    mColor = accColor;
    EmitVertex();
    //16
    gl_Position = p_matrix*mv_matrix*vec4(dece_pos + d_down,gl_in[0].gl_Position.w);
    normal = down;
    aPos = vec4(dece_pos + d_down,gl_in[0].gl_Position.w);
    mColor = deceColor;
    EmitVertex();
    //17
    gl_Position = p_matrix*mv_matrix*vec4(dece_pos + d_left,gl_in[0].gl_Position.w);
    normal = left;
    aPos = vec4(dece_pos + d_left,gl_in[0].gl_Position.w);
    mColor = deceColor;
    EmitVertex();
    }
    //6
    gl_Position = p_matrix*mv_matrix*vec4(curr_pos + d_down,gl_in[1].gl_Position.w);
    normal = down;
    aPos = vec4(curr_pos + d_down,gl_in[1].gl_Position.w);
    mColor = curColor;
    EmitVertex();
    //7
    gl_Position = p_matrix*mv_matrix*vec4(curr_pos + d_left,gl_in[1].gl_Position.w);
    normal = left;
    aPos = vec4(curr_pos + d_left,gl_in[1].gl_Position.w);;
    mColor = curColor;
    EmitVertex();
    //9
    gl_Position = p_matrix*mv_matrix*vec4(curr_pos + d_dir,gl_in[0].gl_Position.w) ;
    normal = dir;
    aPos = vec4(curr_pos + d_dir,gl_in[0].gl_Position.w);
    mColor = curColor;
    EmitVertex();
    EndPrimitive();



    //8
    gl_Position = p_matrix*mv_matrix*vec4(prev_pos - d_dir,gl_in[0].gl_Position.w) ;
    normal = -dir;
    aPos = vec4(prev_pos - d_dir,gl_in[0].gl_Position.w);
    mColor = lastColor;
    EmitVertex();
    //3
    gl_Position = p_matrix*mv_matrix*vec4(prev_pos + d_left,gl_in[0].gl_Position.w);
    normal = left;
    aPos = vec4(prev_pos + d_left,gl_in[0].gl_Position.w);
    mColor = lastColor;
    EmitVertex();
    //0
    gl_Position = p_matrix*mv_matrix*vec4(prev_pos + d_up,gl_in[0].gl_Position.w) ;
    normal = up;
    aPos = vec4(prev_pos + d_up,gl_in[0].gl_Position.w);
    mColor = lastColor;
    EmitVertex();
    if(renderType == 2)
    {
    //13
    gl_Position = p_matrix*mv_matrix*vec4(acc_pos + d_left,gl_in[0].gl_Position.w);
    normal = left;
    aPos = vec4(acc_pos + d_left,gl_in[0].gl_Position.w);
    mColor = accColor;
    EmitVertex();
    //10
    gl_Position = p_matrix*mv_matrix*vec4(acc_pos + d_up,gl_in[0].gl_Position.w) ;
    normal = up;
    aPos = vec4(acc_pos + d_up,gl_in[0].gl_Position.w);
    mColor = accColor;
    EmitVertex();
    //17
    gl_Position = p_matrix*mv_matrix*vec4(dece_pos + d_left,gl_in[0].gl_Position.w);
    normal = left;
    aPos = vec4(dece_pos + d_left,gl_in[0].gl_Position.w);
    mColor = deceColor;
    EmitVertex();
    //14
    gl_Position = p_matrix*mv_matrix*vec4(dece_pos + d_up,gl_in[0].gl_Position.w) ;
    normal = up;
    aPos = vec4(dece_pos + d_up,gl_in[0].gl_Position.w);
    mColor = deceColor;
    EmitVertex();
    }
    //7
    gl_Position = p_matrix*mv_matrix*vec4(curr_pos + d_left,gl_in[1].gl_Position.w);
    normal = left;
    aPos = vec4(curr_pos + d_left,gl_in[1].gl_Position.w);
    mColor = curColor;
    EmitVertex();
    //4
    gl_Position = p_matrix*mv_matrix*vec4(curr_pos + d_up,gl_in[1].gl_Position.w);
    normal = up;
    aPos = vec4(curr_pos + d_up,gl_in[1].gl_Position.w);
    mColor = curColor;
    EmitVertex();
    //9
    gl_Position = p_matrix*mv_matrix*vec4(curr_pos + d_dir,gl_in[0].gl_Position.w) ;
    normal = dir;
    aPos = vec4(curr_pos + d_dir,gl_in[0].gl_Position.w);
    mColor = curColor;
    EmitVertex();
    EndPrimitive();

}
