/****************************************************************************
* VCGLib                                                            o o     *
* Visual and Computer Graphics Library                            o     o   *
*                                                                _   O  _   *
* Copyright(C) 2004-2016                                           \/)\/    *
* Visual Computing Lab                                            /\/|      *
* ISTI - Italian National Research Council                           |      *
*                                                                    \      *
* All rights reserved.                                                      *
*                                                                           *
* This program is free software; you can redistribute it and/or modify      *   
* it under the terms of the GNU General Public License as published by      *
* the Free Software Foundation; either version 2 of the License, or         *
* (at your option) any later version.                                       *
*                                                                           *
* This program is distributed in the hope that it will be useful,           *
* but WITHOUT ANY WARRANTY; without even the implied warranty of            *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
* GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
* for more details.                                                         *
*                                                                           *
****************************************************************************/
/****************************************************************************
  History

$Log: not supported by cvs2svn $

****************************************************************************/

/**
@name Load and Save in Ply format
*/
//@{

#ifndef __VCGLIB_TETRAEXPORT_PLY
#define __VCGLIB_TETRAEXPORT_PLY

#include<wrap/io_tetramesh/io_mask.h>
#include<wrap/io_tetramesh/io_ply.h>
#include<wrap/io_trimesh/precision.h>


#include <stdio.h>

namespace vcg {
namespace tetra {
namespace io {


template <class SaveMeshType>
class ExporterPLY
{
    // Si occupa di convertire da un tipo all'altro.
    // usata nella saveply per matchare i tipi tra stotype e memtype.
    // Ad es se in memoria c'e' un int e voglio salvare un float
    // src sara in effetti un puntatore a int il cui valore deve
    // essere convertito al tipo di ritorno desiderato (stotype)

    template <class StoType>
    static void PlyConv(int mem_type, void *src, StoType &dest)
    {
        switch (mem_type){
        case ply::T_FLOAT	:		dest = (StoType) (*  ((float  *) src)); break;
        case ply::T_DOUBLE  :		dest = (StoType) (*  ((double *) src)); break;
        case ply::T_INT		:		dest = (StoType) (*  ((int    *) src)); break;
        case ply::T_SHORT	:		dest = (StoType) (*  ((short  *) src)); break;
        case ply::T_CHAR	:		dest = (StoType) (*  ((char   *) src)); break;
        case ply::T_UCHAR	:		dest = (StoType) (*  ((unsigned char *)src)); break;
        default : assert(0);
        }
    }

public:
    typedef ::vcg::ply::PropDescriptor PropDescriptor ;
    typedef typename SaveMeshType::VertexPointer VertexPointer;
    typedef typename SaveMeshType::ScalarType ScalarType;
    typedef typename SaveMeshType::VertexType VertexType;
    typedef typename SaveMeshType::TetraType TetraType;
    typedef typename SaveMeshType::TetraPointer TetraPointer;
    typedef typename SaveMeshType::VertexIterator VertexIterator;
    typedef typename SaveMeshType::TetraIterator TetraIterator;

    static bool Save(SaveMeshType &m, const char * filename, bool binary=true)
    {
        PlyInfo pi;
        return Save(m,filename,binary,pi);
    }

    static bool Save(SaveMeshType &m,  const char * filename, int savemask, bool binary=true )
    {
        PlyInfo pi;
        pi.mask=savemask;
        return Save(m,filename,binary,pi);
    }

    static bool Save(SaveMeshType &m,  const char * filename, bool binary, PlyInfo & pi )	// V1.0
    {
        FILE * fpout;
        int i;

        const char * hbin = "binary_little_endian";
        const char * hasc = "ascii";
        const char * h;

        bool multit = false;

        const int DGT   = vcg::tri::io::Precision<ScalarType>::digits();
        const int DGTVQ = vcg::tri::io::Precision<typename VertexType::QualityType>::digits();
        const int DGTVR = vcg::tri::io::Precision<typename VertexType::RadiusType>::digits();
        const int DGTTQ = vcg::tri::io::Precision<typename TetraType::QualityType>::digits();


        if(binary) h=hbin;
        else       h=hasc;

        fpout = fopen(filename,"wb");
        if(fpout==NULL) {
            pi.status=::vcg::ply::E_CANTOPEN;
            return ::vcg::ply::E_CANTOPEN;
        }

        fprintf(fpout,
                "ply\n"
                "format %s 1.0\n"
                "comment VCGLIB generated\n"
                ,h
                );

        //if( pi.mask & ply::PLYMask::PM_WEDGTEXCOORD )
        //{
        //	//const char * TFILE = "TextureFile";

        //	//for(i=0;i<textures.size();++i)
        //	//	fprintf(fpout,"comment %s %s\n", TFILE, (const char *)(textures[i]) );

        //	//if(textures.size()>1 && (HasPerWedgeTexture() || HasPerVertexTexture())) multit = true;
        //}

        //if( (pi.mask & PLYMask::PM_CAMERA) && camera.IsValid() )
        /*{
                fprintf(fpout,
                        "element camera 1\n"
                        "property float view_px\n"
                        "property float view_py\n"
                        "property float view_pz\n"
                        "property float x_axisx\n"
                        "property float x_axisy\n"
                        "property float x_axisz\n"
                        "property float y_axisx\n"
                        "property float y_axisy\n"
                        "property float y_axisz\n"
                        "property float z_axisx\n"
                        "property float z_axisy\n"
                        "property float z_axisz\n"
                        "property float focal\n"
                        "property float scalex\n"
                        "property float scaley\n"
                        "property float centerx\n"
                        "property float centery\n"
                        "property int viewportx\n"
                        "property int viewporty\n"
                        "property float k1\n"
                        "property float k2\n"
                        "property float k3\n"
                        "property float k4\n"
                );
        }*/

        // VERT
        const char* vttp = vcg::tri::io::Precision<ScalarType>::typeName();
        fprintf(fpout,"element vertex %d\n",m.vn);
        fprintf(fpout,"property %s x\n",vttp);
        fprintf(fpout,"property %s y\n",vttp);
        fprintf(fpout,"property %s z\n",vttp);


        if( HasPerVertexFlags(m) &&( pi.mask & Mask::IOM_VERTFLAGS) )
        {
            fprintf(fpout,
                "property int flags\n"
                );
        }

        if( HasPerVertexColor(m)  && (pi.mask & Mask::IOM_VERTCOLOR) )
        {
            fprintf(fpout,
                    "property uchar red\n"
                    "property uchar green\n"
                    "property uchar blue\n"
                    "property uchar alpha\n"
                    );
        }

        if( HasPerVertexQuality(m) && (pi.mask & Mask::IOM_VERTQUALITY) )
        {
            const char* vqtp = vcg::tri::io::Precision<typename VertexType::ScalarType>::typeName();
            fprintf(fpout,"property %s quality\n",vqtp);
        }

        for(i=0;i<pi.vdn;i++)
            fprintf(fpout,"property %s %s\n",pi.VertexData[i].stotypename(),pi.VertexData[i].propname);

// TETRA
        fprintf(fpout,
                "element tetra %d\n"
                "property list uchar int vertex_indices\n"
                ,m.tn
                );

        if( pi.mask & Mask::IOM_TETRAFLAGS)
        {
            fprintf(fpout,
                    "property int flags\n"
                    );
        }

        if( (pi.mask & Mask::IOM_TETRACOLOR) )
        {
            fprintf(fpout,
                    "property uchar red\n"
                    "property uchar green\n"
                    "property uchar blue\n"
                    "property uchar alpha\n"
                    );
        }

        if( (pi.mask & Mask::IOM_TETRAQUALITY) )
        {
                const char* vqtp = vcg::tri::io::Precision<typename TetraType::ScalarType>::typeName();
                fprintf(fpout,"property %s quality\n",vqtp);
        }

        for(i=0;i<pi.fdn;i++)
            fprintf(fpout,"property %s %s\n",pi.TetraData[i].stotypename(),pi.TetraData[i].propname);

        fprintf(fpout, "end_header\n"	);

        // Salvataggio camera
        //if( (pi.mask & ply::PLYMask::PM_CAMERA) && camera.IsValid() )
        //{
        //if(binary)
        //{
        //	float t[17];

        //	t[ 0] = camera.view_p[0];
        //	t[ 1] = camera.view_p[1];
        //	t[ 2] = camera.view_p[2];
        //	t[ 3] = camera.x_axis[0];
        //	t[ 4] = camera.x_axis[1];
        //	t[ 5] = camera.x_axis[2];
        //	t[ 6] = camera.y_axis[0];
        //	t[ 7] = camera.y_axis[1];
        //	t[ 8] = camera.y_axis[2];
        //	t[ 9] = camera.z_axis[0];
        //	t[10] = camera.z_axis[1];
        //	t[11] = camera.z_axis[2];
        //	t[12] = camera.f;
        //	t[13] = camera.s[0];
        //	t[14] = camera.s[1];
        //	t[15] = camera.c[0];
        //	t[16] = camera.c[1];
        //	fwrite(t,sizeof(float),17,fpout);

        //	fwrite( camera.viewport,sizeof(int),2,fpout );

        //	t[ 0] = camera.k[0];
        //	t[ 1] = camera.k[1];
        //	t[ 2] = camera.k[2];
        //	t[ 3] = camera.k[3];
        //	fwrite(t,sizeof(float),4,fpout);
        //}
        //else
        //{
        //	fprintf(fpout,"%g %g %g %g %g %g %g %g %g %g %g %g %g %g %g %g %g %d %d %g %g %g %g\n"
        //		,camera.view_p[0]
        //		,camera.view_p[1]
        //		,camera.view_p[2]
        //		,camera.x_axis[0]
        //		,camera.x_axis[1]
        //		,camera.x_axis[2]
        //		,camera.y_axis[0]
        //		,camera.y_axis[1]
        //		,camera.y_axis[2]
        //		,camera.z_axis[0]
        //		,camera.z_axis[1]
        //		,camera.z_axis[2]
        //		,camera.f
        //		,camera.s[0]
        //		,camera.s[1]
        //		,camera.c[0]
        //		,camera.c[1]
        //		,camera.viewport[0]
        //		,camera.viewport[1]
        //		,camera.k[0]
        //		,camera.k[1]
        //		,camera.k[2]
        //		,camera.k[3]
        //	);
        //}
        //}


        SimpleTempData<typename SaveMeshType::VertContainer,int> indices(m.vert);

        int j;
        std::vector<int> FlagV;
        VertexPointer  vp;
        VertexIterator vi;
        for(j=0,vi=m.vert.begin();vi!=m.vert.end();++vi)
        {
            vp=&(*vi);
            indices[vi] = j;
            FlagV.push_back(vp->Flags()); // Salva in ogni caso flag del vertice
            if( ! vp->IsD() )
            {
                if(binary)
                {
                    float t;

                    t = float(vp->P()[0]); fwrite(&t,sizeof(float),1,fpout);
                    t = float(vp->P()[1]); fwrite(&t,sizeof(float),1,fpout);
                    t = float(vp->P()[2]); fwrite(&t,sizeof(float),1,fpout);

                    if( pi.mask & Mask::IOM_VERTFLAGS )
                        fwrite(&(vp->Flags()),sizeof(int),1,fpout);

                    if( HasPerVertexColor(m) && (pi.mask & Mask::IOM_VERTCOLOR) )
                        fwrite(&( vp->C() ),sizeof(char),4,fpout);

                    if( HasPerVertexQuality(m) && (pi.mask & Mask::IOM_VERTQUALITY) )
                        fwrite(&( vp->Q() ),DGTVQ,1,fpout);


                    for(i=0;i<pi.vdn;i++)
                    {
                        double td; float tf;int ti;short ts; char tc; unsigned char tuc;
                        switch (pi.VertexData[i].stotype1)
                        {
                        case ply::T_FLOAT	 :		PlyConv(pi.VertexData[i].memtype1,  ((char *)vp)+pi.VertexData[i].offset1, tf );	fwrite(&tf, sizeof(float),1,fpout); break;
                        case ply::T_DOUBLE       :		PlyConv(pi.VertexData[i].memtype1,  ((char *)vp)+pi.VertexData[i].offset1, td );	fwrite(&td, sizeof(double),1,fpout); break;
                        case ply::T_INT		 :		PlyConv(pi.VertexData[i].memtype1,  ((char *)vp)+pi.VertexData[i].offset1, ti );	fwrite(&ti, sizeof(int),1,fpout); break;
                        case ply::T_SHORT	 :		PlyConv(pi.VertexData[i].memtype1,  ((char *)vp)+pi.VertexData[i].offset1, ts );	fwrite(&ts, sizeof(short),1,fpout); break;
                        case ply::T_CHAR	 :		PlyConv(pi.VertexData[i].memtype1,  ((char *)vp)+pi.VertexData[i].offset1, tc );	fwrite(&tc, sizeof(char),1,fpout); break;
                        case ply::T_UCHAR	 :		PlyConv(pi.VertexData[i].memtype1,  ((char *)vp)+pi.VertexData[i].offset1, tuc);	fwrite(&tuc,sizeof(unsigned char),1,fpout); break;
                        default : assert(0);
                        }
                    }
                }
                else 	// ***** ASCII *****
                {
                    fprintf(fpout,"%g %g %g " ,vp->P()[0],vp->P()[1],vp->P()[2]);

                    if( pi.mask & Mask::IOM_VERTFLAGS )
                        fprintf(fpout,"%d ",vp->Flags());

                    if( HasPerVertexColor(m) && (pi.mask & Mask::IOM_VERTCOLOR) )
                        fprintf(fpout,"%d %d %d %d ",vp->C()[0], vp->C()[1], vp->C()[2], vp->C()[3] );

                    if( HasPerVertexQuality(m) && (pi.mask & Mask::IOM_VERTQUALITY) )
                        fprintf(fpout,"%g ",vp->Q());

                    for(i=0;i<pi.vdn;i++)
                    {
                        float tf; double td;
                        int ti;
                        switch (pi.VertexData[i].memtype1)
                        {
                        case ply::T_FLOAT	 :		tf=*( (float  *)        (((char *)vp)+pi.VertexData[i].offset1));	fprintf(fpout,"%g ",tf); break;
                        case ply::T_DOUBLE :    td=*( (double *)        (((char *)vp)+pi.VertexData[i].offset1));	fprintf(fpout,"%g ",tf); break;
                        case ply::T_INT		 :		ti=*( (int    *)        (((char *)vp)+pi.VertexData[i].offset1));	fprintf(fpout,"%i ",ti); break;
                        case ply::T_SHORT	 :		ti=*( (short  *)        (((char *)vp)+pi.VertexData[i].offset1)); fprintf(fpout,"%i ",ti); break;
                        case ply::T_CHAR	 :		ti=*( (char   *)        (((char *)vp)+pi.VertexData[i].offset1));	fprintf(fpout,"%i ",ti); break;
                        case ply::T_UCHAR	 :		ti=*( (unsigned char *) (((char *)vp)+pi.VertexData[i].offset1));	fprintf(fpout,"%i ",ti); break;
                        default : assert(0);
                        }
                    }

                    fprintf(fpout,"\n");
                }

                vp->Flags()=j; // Trucco! Nascondi nei flags l'indice del vertice non deletato!
                j++;
            }
        }
        assert(j==m.vn);

        char c = 4;
        unsigned char b9 = 9;
        unsigned char b6 = 6;
        TetraPointer fp;
        int vv[4];
        TetraIterator fi;
        int fcnt=0;
        for(j=0,fi=m.tetra.begin();fi!=m.tetra.end();++fi)
        {
            fp=&(*fi);
            if( ! fp->IsD() )
            { fcnt++;
                if(binary)
                {
                    vv[0]=indices[fp->cV(0)];
                    vv[1]=indices[fp->cV(1)];
                    vv[2]=indices[fp->cV(2)];
                    vv[3]=indices[fp->cV(3)];


                    fwrite(&c,1,1,fpout);
                    fwrite(vv,sizeof(int),4,fpout);

                    if( pi.mask & Mask::IOM_TETRAFLAGS )
                        fwrite(&(fp->Flags()),sizeof(int),1,fpout);

                    if( (pi.mask & Mask::IOM_TETRACOLOR) )
                        fwrite(&( fp->C() ), sizeof(char),4,fpout);

                    if( (pi.mask & Mask::IOM_TETRAQUALITY) )
                        fwrite( &(fp->Q()), DGTTQ,1,fpout);


                    for(i=0;i<pi.fdn;i++)
                    {
                        double td; float tf;int ti;short ts; char tc; unsigned char tuc;
                        switch (pi.TetraData[i].stotype1){
                        case ply::T_FLOAT	 :		PlyConv(pi.TetraData[i].memtype1,  ((char *)fp)+pi.TetraData[i].offset1, tf );	fwrite(&tf, sizeof(float),1,fpout); break;
                        case ply::T_DOUBLE :		PlyConv(pi.TetraData[i].memtype1,  ((char *)fp)+pi.TetraData[i].offset1, td );	fwrite(&td, sizeof(double),1,fpout); break;
                        case ply::T_INT		 :		PlyConv(pi.TetraData[i].memtype1,  ((char *)fp)+pi.TetraData[i].offset1, ti );	fwrite(&ti, sizeof(int),1,fpout); break;
                        case ply::T_SHORT	 :		PlyConv(pi.TetraData[i].memtype1,  ((char *)fp)+pi.TetraData[i].offset1, ts );	fwrite(&ts, sizeof(short),1,fpout); break;
                        case ply::T_CHAR	 :		PlyConv(pi.TetraData[i].memtype1,  ((char *)fp)+pi.TetraData[i].offset1, tc );	fwrite(&tc, sizeof(char),1,fpout); break;
                        case ply::T_UCHAR	 :		PlyConv(pi.TetraData[i].memtype1,  ((char *)fp)+pi.TetraData[i].offset1, tuc);	fwrite(&tuc,sizeof(unsigned char),1,fpout); break;
                        default : assert(0);
                        }
                    }
                }
                else	// ***** ASCII *****
                {
                    fprintf(fpout,"%d " , 4);
                    fprintf(fpout,"%d %d %d %d ",
                            indices[fp->cV(0)],	indices[fp->cV(1)], indices[fp->cV(2)], indices[fp->cV(3)]);

                    if( pi.mask & Mask::IOM_TETRAFLAGS )
                        fprintf(fpout,"%d ",fp->Flags());

                    if( TetraType::HasColor() && (pi.mask & Mask::IOM_TETRACOLOR)  )
                        fprintf(fpout,"%d %d %d %d ",vp->C()[0],vp->C()[1],vp->C()[2],vp->C()[3] );


                    if((pi.mask & Mask::IOM_TETRAQUALITY) )
                        fprintf(fpout,"%g ",fp->Q());

                    for(i=0;i<pi.fdn;i++)
                    {
                        float tf; double td;
                        int ti;
                        switch (pi.TetraData[i].memtype1)
                        {
                        case  ply::T_FLOAT	:		tf=*( (float  *)        (((char *)fp)+pi.TetraData[i].offset1));	fprintf(fpout,"%g ",tf); break;
                        case  ply::T_DOUBLE :		td=*( (double *)        (((char *)fp)+pi.TetraData[i].offset1));	fprintf(fpout,"%g ",tf); break;
                        case  ply::T_INT		:		ti=*( (int    *)        (((char *)fp)+pi.TetraData[i].offset1));	fprintf(fpout,"%i ",ti); break;
                        case  ply::T_SHORT	:		ti=*( (short  *)        (((char *)fp)+pi.TetraData[i].offset1));	fprintf(fpout,"%i ",ti); break;
                        case  ply::T_CHAR		:		ti=*( (char   *)        (((char *)fp)+pi.TetraData[i].offset1));	fprintf(fpout,"%i ",ti); break;
                        case  ply::T_UCHAR	:		ti=*( (unsigned char *) (((char *)fp)+pi.TetraData[i].offset1));	fprintf(fpout,"%i ",ti); break;
                        default : assert(0);
                        }
                    }

                    fprintf(fpout,"\n");
                }
            }
        }
        assert(fcnt==m.tn);
        fclose(fpout);

        // Recupera i flag originali
        for(j=0,vi=m.vert.begin();vi!=m.vert.end();++vi)
            (*vi).Flags()=FlagV[j++];

        return 0;
    }




}; // end class



} // end namespace tetra
} // end namespace io
} // end namespace vcg

#endif
