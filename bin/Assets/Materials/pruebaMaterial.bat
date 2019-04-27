@echo off

set /A Counter=1
echo Initial value of Counter: %Counter%
set /A Counter=Counter+1
echo Next value of Counter: %Counter%

for %%f in (*.material) do (
    
    echo READING
    setlocal enabledelayedexpansion
    
    for /f "tokens=*" %%a in ('type donut.material') do (
        set line=%%a
        echo !line!
    )

    echo END READING

    endlocal
	break>"%%~nf.material"

	echo vertex_program toonVs%%~nf glsl >>"%%~nf.material"
	echo { >>"%%~nf.material"
    echo source toonVs.glsl >>"%%~nf.material"
    echo default_params >>"%%~nf.material"
    echo { >>"%%~nf.material"
    echo param_named_auto modelViewProjMat worldviewproj_matrix >>"%%~nf.material"
    echo param_named_auto normalMat inverse_transpose_world_matrix >>"%%~nf.material" 
    echo } >>"%%~nf.material"
	echo } >>"%%~nf.material"


	echo fragment_program toonFs%%~nf glsl >>"%%~nf.material"
	echo { >>"%%~nf.material" 
    echo source toonFs.glsl >>"%%~nf.material"
    echo default_params >>"%%~nf.material"
    echo { >>"%%~nf.material"
    echo param_named textura int 0 >>"%%~nf.material"
    echo param_named_auto lightDiffuse light_diffuse_colour 0 >>"%%~nf.material"
    echo param_named_auto lightDirection light_position 0 >>"%%~nf.material" 
    echo param_named shadow_level float 0.2 >>"%%~nf.material"
    echo param_named shadow_color float 0.2 >>"%%~nf.material"

    echo } >>"%%~nf.material"
	echo } >>"%%~nf.material" 



	echo material %%~nf >>"%%~nf.material"
	echo { >>"%%~nf.material"
    echo receive_shadows on >>"%%~nf.material" 

    echo technique >>"%%~nf.material"
    echo { >>"%%~nf.material"
    echo pass tabla_de_quesos >>"%%~nf.material"
    echo { >>"%%~nf.material"

    echo  cull_hardware none >>"%%~nf.material"
    echo  cull_software none >>"%%~nf.material"

    echo  vertex_program_ref toonVs%%~nf >>"%%~nf.material"
    echo  { >>"%%~nf.material"
    echo  } >>"%%~nf.material"
    echo  fragment_program_ref toonFs%%~nf >>"%%~nf.material"
    echo  { >>"%%~nf.material"
    echo  param_named color float3 0.24 0.7 0.4 >>"%%~nf.material"
    echo  } >>"%%~nf.material"

    echo ambient 0.14238853752613068 0.06427600234746933 0.029756436124444008 1.0 >>"%%~nf.material"
    echo diffuse 0.11391083171830818 0.05142080264420512 0.023805149254279567 1.0 >>"%%~nf.material"
    echo specular 0.5 0.5 0.5 1.0 12.5 >>"%%~nf.material"
    echo emissive 0.0 0.0 0.0 1.0 >>"%%~nf.material"
    echo alpha_to_coverage off >>"%%~nf.material"
    echo colour_write on >>"%%~nf.material"
    echo cull_hardware clockwise >>"%%~nf.material"
    echo depth_check on >>"%%~nf.material"
    echo depth_func less_equal >>"%%~nf.material"
    echo depth_write on >>"%%~nf.material"
    echo illumination_stage >>"%%~nf.material"
    echo light_clip_planes off >>"%%~nf.material"
    echo light_scissor off >>"%%~nf.material" 
    echo lighting on >>"%%~nf.material"
    echo normalise_normals off >>"%%~nf.material"
    echo polygon_mode solid >>"%%~nf.material"
    echo scene_blend one zero >>"%%~nf.material"
    echo scene_blend_op add >>"%%~nf.material"
    echo shading gouraud >>"%%~nf.material"
    echo transparent_sorting on >>"%%~nf.material"

    echo } >>"%%~nf.material" 
    echo } >>"%%~nf.material"
	echo } >>"%%~nf.material"
)	