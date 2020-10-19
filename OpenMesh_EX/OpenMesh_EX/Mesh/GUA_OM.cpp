#include "GUA_OM.h"

#define DEBUG
double PointAngle(Tri_Mesh::Point P1, Tri_Mesh::Point P2, Tri_Mesh::Point VPoint);
namespace OMT
{
	/*======================================================================*/
	Model::Model()
	{
		request_vertex_status();
		request_edge_status();
		request_face_status();
	}
	Model::~Model()
	{
		release_vertex_status();
		release_edge_status();
		release_face_status();
	}
}
/*======================================================================*/
namespace OMP
{
	Model::Model()
	{
		Mesh.request_vertex_status();
		Mesh.request_edge_status();
		Mesh.request_face_status();
	}
	Model::~Model()
	{
		Mesh.release_vertex_status();
		Mesh.release_edge_status();
		Mesh.release_face_status();
	}
	/*======================================================================*/
	bool Model::ReadFile(std::string _fileName)
	{
		bool isRead = false;
		OpenMesh::IO::Options opt;
		if (OpenMesh::IO::read_mesh(Mesh, _fileName, opt))
		{
			//read mesh from filename OK!
			isRead = true;
		}
		if (isRead)
		{
			// If the file did not provide vertex normals and mesh has vertex normal ,then calculate them
			if (!opt.check(OpenMesh::IO::Options::VertexNormal) && Mesh.has_vertex_normals())
			{
				Mesh.update_normals();
			}
		}
		return isRead;
	}
	bool Model::SaveFile(std::string _fileName)
	{
		bool isSave = false;
		OpenMesh::IO::Options opt;
		if (OpenMesh::IO::write_mesh(Mesh, _fileName, opt))
		{
			//read mesh from filename OK!
			isSave = true;
		}
		return isSave;
	}
	/*======================================================================*/
	void Model::Render_solid()
	{
		FIter f_it;
		FVIter	fv_it;
		glPushAttrib(GL_LIGHTING_BIT);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glEnable(GL_DEPTH_TEST);
		glPolygonOffset(2.0, 2.0);
		glBegin(GL_POLYGON);
		//glColor4f(1.0, 0.5, 1.0, 0.5);
		for (f_it = Mesh.faces_begin(); f_it != Mesh.faces_end(); ++f_it)
		{
			for (fv_it = Mesh.fv_iter(f_it); fv_it; ++fv_it)
			{
				glNormal3dv(Mesh.normal(fv_it.handle()).data());
				glVertex3dv(Mesh.point(fv_it.handle()).data());
			}
		}
		glEnd();
		glDisable(GL_POLYGON_OFFSET_FILL);
	}
	void Model::Render_wireframe()
	{
		Tri_Mesh::HalfedgeHandle _hedge;
		EIter e_it = Mesh.edges_begin();

		glDisable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
		glColor3f(0.0, 0.0, 0.0);
		glLineWidth(1);
		glBegin(GL_LINES);
		for (e_it = Mesh.edges_begin(); e_it != Mesh.edges_end(); ++e_it)
		{
			_hedge = Mesh.halfedge_handle(e_it.handle(), 1);

			glVertex3dv(Mesh.point(Mesh.from_vertex_handle(_hedge)).data());
			glVertex3dv(Mesh.point(Mesh.to_vertex_handle(_hedge)).data());
		}
		glEnd();
		glEnable(GL_LIGHTING);

	}
	/*======================================================================*/
	void Model::RenderSpecifiedPoint()
	{
		glPushAttrib(GL_LIGHTING_BIT);
		glDisable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
		glPointSize(5.0f);
		glBegin(GL_POINTS);
		vector<sp_p>::iterator p_itr = sp_p_list.begin();
		for (p_itr; p_itr != sp_p_list.end(); ++p_itr)
		{
			glColor3f(p_itr->r, p_itr->g, p_itr->b);
			glVertex3dv(p_itr->pt.data());
		}
		glEnd();
		glEnable(GL_LIGHTING);
		glDisable(GL_POLYGON_OFFSET_FILL);
	}
	void Model::RenderSpecifiedVertex()
	{
		glPushAttrib(GL_LIGHTING_BIT);
		glDisable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
		glPointSize(5.0f);
		glBegin(GL_POINTS);
		vector< sp_v >::iterator v_itr = sp_v_list.begin();
		for (v_itr; v_itr != sp_v_list.end(); ++v_itr)
		{
			glColor3f(v_itr->r, v_itr->g, v_itr->b);
			glVertex3dv(Mesh.point(v_itr->vh).data());
		}
		glEnd();
		glEnable(GL_LIGHTING);
		glDisable(GL_POLYGON_OFFSET_FILL);
	}
	void Model::RenderSpecifiedFace()
	{
		glDisable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glPushAttrib(GL_LIGHTING_BIT);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(0.5, 1.0);
		glDisable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
		glBegin(GL_QUADS);
		FVIter fv_itr;
		vector< sp_f >::iterator f_itr;
		for (f_itr = sp_f_list.begin(); f_itr != sp_f_list.end(); ++f_itr)
		{
			glColor3f(f_itr->r, f_itr->g, f_itr->b);
			for (fv_itr = Mesh.fv_iter(f_itr->fh); fv_itr; ++fv_itr)
			{
				glNormal3dv(Mesh.normal(fv_itr.handle()).data());
				glVertex3dv(Mesh.point(fv_itr.handle()).data());
			}
		}
		glEnd();
		glEnable(GL_LIGHTING);
		glDisable(GL_POLYGON_OFFSET_FILL);
		glPolygonMode(GL_FRONT, GL_FILL);
		glEnable(GL_CULL_FACE);
	}

	/*======================================================================*/
	void Model::add_sp_p(Point   _p, float _r, float _g, float _b)
	{
		sp_p input_data;
		input_data.pt = _p;
		input_data.r = _r;
		input_data.g = _g;
		input_data.b = _b;
		sp_p_list.push_back(input_data);
	}
	void Model::add_sp_v(VHandle _v, float _r, float _g, float _b)
	{
		sp_v input_data;
		input_data.vh = _v;
		input_data.r = _r;
		input_data.g = _g;
		input_data.b = _b;
		sp_v_list.push_back(input_data);
	}
	void Model::add_sp_f(FHandle _f, float _r, float _g, float _b)
	{
		sp_f input_data;
		input_data.fh = _f;
		input_data.r = _r;
		input_data.g = _g;
		input_data.b = _b;
		sp_f_list.push_back(input_data);
	}
	void Model::clear_sp_p()
	{
		sp_p_list.clear();
	}
	void Model::clear_sp_v()
	{
		sp_v_list.clear();
	}
	void Model::clear_sp_f()
	{
		sp_f_list.clear();
	}
	/*======================================================================*/
	VHandle Model::addVertex(Point _p)
	{
		int find_result = findVertex(_p);
		if (find_result != -1)
		{
			return Mesh.vertex_handle(find_result);
		}
		else
		{
			return Mesh.add_vertex(_p);
		}
	}
	FHandle Model::addFace(VHandle _v0, VHandle _v1, VHandle _v2, VHandle _v3)
	{
		vector<VHandle> face_vhandles;

		face_vhandles.clear();
		face_vhandles.push_back(_v0);
		face_vhandles.push_back(_v1);
		face_vhandles.push_back(_v2);
		face_vhandles.push_back(_v3);
		return Mesh.add_face(face_vhandles);
	}
	void Model::deleteFace(FHandle _f)
	{
		Mesh.delete_face(_f);
		Mesh.garbage_collection();
	}
	void Model::deleteFace(VHandle _v0, VHandle _v1, VHandle _v2, VHandle _v3)
	{
		/*
		v1				v0
		*<--------------*
		|				|
		|				|
		|				|
		|		f		|
		|				|
		|				|
		|				|
		* --------------*
		v2				v3
		*/

		HEHandle v0v1 = Mesh.find_halfedge(_v0, _v1);
		if (v0v1.is_valid())
		{
			FHandle fh = Mesh.face_handle(v0v1);
			if (fh.is_valid())
			{
				Mesh.delete_face(fh);
				Mesh.garbage_collection();
			}
		}
	}
	/*======================================================================*/
	bool Model::IsVertexVertex(VHandle _vj, VHandle _vi)
	{
		for (VVIter vvit = Mesh.vv_iter(_vi); vvit; ++vvit)
			if (vvit.handle() == _vj)
				return true;
		return false;
	}
	/*======================================================================*/
	int Model::quad_subdivision1(int _face_id)
	{
		/*----------------------------------------------------------------------*/
		//這段是為了解決index問題
		VHandle vq, vw, vt, vr;
		vq = addVertex(Point(0, 0, 100));
		vw = addVertex(Point(1, 0, 100));
		vt = addVertex(Point(1, 1, 100));
		vr = addVertex(Point(0, 1, 100));
		addFace(vq, vw, vt, vr);
		/*----------------------------------------------------------------------*/
		/*收集需要subdivision的face*/
		//初始table
		bool *table = new bool[Mesh.n_faces()];
		for (unsigned i = 0; i < Mesh.n_faces(); i++)
		{
			table[i] = false;
		}

		vector< FHandle > candidate_faces, last_found_face;
		last_found_face.push_back(Mesh.face_handle(_face_id));
		table[_face_id] = true;

		while (last_found_face.size() != 0)
		{
			vector< FHandle > new_found_faces;
			for (vector< FHandle >::iterator crnt_f = last_found_face.begin(); crnt_f != last_found_face.end(); ++crnt_f)
			{
				for (FFIter ff_itr = Mesh.ff_iter(*crnt_f); ff_itr; ++ff_itr)
				{
					if (table[ff_itr.handle().idx()] != true)
					{
						new_found_faces.push_back(ff_itr.handle());
						table[ff_itr.handle().idx()] = true;
					}
				}
			}
			for (vector< FHandle >::iterator f_itr = last_found_face.begin(); f_itr != last_found_face.end(); ++f_itr)
			{
				candidate_faces.push_back(*f_itr);
			}
			last_found_face.assign(new_found_faces.begin(), new_found_faces.end());
		}
		delete table;
		/*----------------------------------------------------------------------*/
		/*對candidate faces做subdivision*/
		/*
			v0		vd		v3
			*-------*-------*
			|		|		|
			|		|		|
			|	  ve|		|
		  va*-------*-------*vc
			|		|		|
			|		|		|
			|		|		|
			*-------*-------*
			v1		vb		v2
		*/
		for (vector< FHandle >::iterator face_itr = candidate_faces.begin(); face_itr != candidate_faces.end(); ++face_itr)
		{
			VHandle v[4], va, vb, vc, vd, ve;
			FVIter fv_itr = Mesh.fv_iter(*face_itr);
			for (int i = 0; fv_itr; ++fv_itr)
			{
				v[i++] = fv_itr.handle();
			}

			deleteFace(v[0], v[1], v[2], v[3]);
			va = addVertex((Mesh.point(v[0]) + Mesh.point(v[1])) / 2);
			vb = addVertex((Mesh.point(v[1]) + Mesh.point(v[2])) / 2);
			vc = addVertex((Mesh.point(v[2]) + Mesh.point(v[3])) / 2);
			vd = addVertex((Mesh.point(v[3]) + Mesh.point(v[0])) / 2);
			ve = addVertex((Mesh.point(v[0]) + Mesh.point(v[1]) + Mesh.point(v[2]) + Mesh.point(v[3])) / 4);
			addFace(vd, v[0], va, ve);
			addFace(va, v[1], vb, ve);
			addFace(vb, v[2], vc, ve);
			addFace(vc, v[3], vd, ve);
		}
		/*----------------------------------------------------------------------*/
		deleteFace(vq, vw, vt, vr);//這行只是為了解決index問題
		/*----------------------------------------------------------------------*/
		return 0;
	}
	int Model::quad_subdivision2(int _face_id)
	{
		/*----------------------------------------------------------------------*/
		//這段是為了解決index問題
		VHandle vq, vw, vt, vr;
		vq = addVertex(Point(0, 0, 100));
		vw = addVertex(Point(1, 0, 100));
		vt = addVertex(Point(1, 1, 100));
		vr = addVertex(Point(0, 1, 100));
		addFace(vq, vw, vt, vr);
		/*----------------------------------------------------------------------*/
		/*收集需要subdivision的face*/
		//初始table
		bool *table = new bool[Mesh.n_faces()];
		for (unsigned i = 0; i < Mesh.n_faces(); i++)
		{
			table[i] = false;
		}

		vector< FHandle > candidate_faces, last_found_face;
		last_found_face.push_back(Mesh.face_handle(_face_id));
		table[_face_id] = true;

		while (last_found_face.size() != 0)
		{
			vector< FHandle > new_found_faces;
			for (vector< FHandle >::iterator crnt_f = last_found_face.begin(); crnt_f != last_found_face.end(); ++crnt_f)
			{
				for (FFIter ff_itr = Mesh.ff_iter(*crnt_f); ff_itr; ++ff_itr)
				{
					if (table[ff_itr.handle().idx()] != true)
					{
						new_found_faces.push_back(ff_itr.handle());
						table[ff_itr.handle().idx()] = true;
					}
				}
			}
			for (vector< FHandle >::iterator f_itr = last_found_face.begin(); f_itr != last_found_face.end(); ++f_itr)
			{
				candidate_faces.push_back(*f_itr);
			}
			last_found_face.assign(new_found_faces.begin(), new_found_faces.end());
		}
		delete table;
		/*----------------------------------------------------------------------*/
		/*對candidate faces做subdivision*/
		/*
			v0		vh		vg		v3
			*-------*-------*-------*
			|		|		|		|
			|		|		|		|
			|	  vi|	  vl|		|
		 va *-------*-------*-------*vf
			|		|		|		|
			|		|		|		|
			|	  vj|	  vk|		|
		 vb *-------*-------*-------*ve
			|		|		|		|
			|		|		|		|
			|		|		|		|
			*-------*-------*-------*
			v1		vc		vd		v2
		*/
		for (vector< FHandle >::iterator face_itr = candidate_faces.begin(); face_itr != candidate_faces.end(); ++face_itr)
		{
			VHandle v[4], va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl;
			FVIter fv_itr = Mesh.fv_iter(*face_itr);
			for (int i = 0; fv_itr; ++fv_itr)
			{
				v[i++] = fv_itr.handle();
			}

			deleteFace(v[0], v[1], v[2], v[3]);
			va = addVertex((Mesh.point(v[0]) * 2 + Mesh.point(v[1])) / 3);
			vb = addVertex((Mesh.point(v[0]) + Mesh.point(v[1]) * 2) / 3);
			vc = addVertex((Mesh.point(v[1]) * 2 + Mesh.point(v[2])) / 3);
			vd = addVertex((Mesh.point(v[1]) + Mesh.point(v[2]) * 2) / 3);
			ve = addVertex((Mesh.point(v[2]) * 2 + Mesh.point(v[3])) / 3);
			vf = addVertex((Mesh.point(v[2]) + Mesh.point(v[3]) * 2) / 3);
			vg = addVertex((Mesh.point(v[3]) * 2 + Mesh.point(v[0])) / 3);
			vh = addVertex((Mesh.point(v[3]) + Mesh.point(v[0]) * 2) / 3);

			vi = addVertex((Mesh.point(vh) * 2 + Mesh.point(vc)) / 3);
			vj = addVertex((Mesh.point(vh) + Mesh.point(vc) * 2) / 3);
			vk = addVertex((Mesh.point(vd) * 2 + Mesh.point(vg)) / 3);
			vl = addVertex((Mesh.point(vd) + Mesh.point(vg) * 2) / 3);
			addFace(v[0], va, vi, vh);
			addFace(va, vb, vj, vi);
			addFace(vb, v[1], vc, vj);
			addFace(vc, vd, vk, vj);
			addFace(vd, v[2], ve, vk);
			addFace(ve, vf, vl, vk);
			addFace(vf, v[3], vg, vl);
			addFace(vg, vh, vi, vl);
			addFace(vi, vj, vk, vl);
		}
		/*----------------------------------------------------------------------*/
		deleteFace(vq, vw, vt, vr);//這行只是為了解決index問題
		/*----------------------------------------------------------------------*/
		return 0;
	}
	/*======================================================================*/
	int Model::findVertex(Point _p)
	{
		for (VIter v_itr = Mesh.vertices_begin(); v_itr != Mesh.vertices_end(); ++v_itr)
			if (Mesh.point(v_itr) == _p)
				return v_itr.handle().idx();
		return -1;
	}
	/*======================================================================*/
};
/*======================================================================*/
void Tri_Mesh::Render_Solid()
{
	FIter f_it;
	FVIter	fv_it;
	//glPushAttrib(GL_LIGHTING_BIT);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glEnable(GL_LIGHTING);
	glPolygonOffset(2.0, 2.0);
	glBegin(GL_TRIANGLES);
	glColor4f(0.81, 0.74, 0.33, 0.3);
	for (f_it = faces_begin(); f_it != faces_end(); ++f_it)
	{
		for (fv_it = fv_iter(f_it); fv_it; ++fv_it)
		{
			glNormal3dv(normal(fv_it.handle()).data());
			glVertex3dv(point(fv_it.handle()).data());
		}
	}
	glEnd();

	glDisable(GL_POLYGON_OFFSET_FILL);
}

void Tri_Mesh::Render_SolidWireframe()
{
	FIter f_it;
	FVIter	fv_it;

	/*glPointSize(8.0);
	glBegin(GL_POINTS);
	glColor4f(1.0, 0.0, 0.0, 1.0);
	glVertex3dv(point(vertex_handle(0)));
	glEnd();*/
	glDisable(GL_LIGHTING);
	glPushAttrib(GL_LIGHTING_BIT);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glEnable(GL_DEPTH_TEST);
	glPolygonOffset(2.0, 2.0);
	glBegin(GL_TRIANGLES);
	glColor4f(1.0, 0.96, 0.49, 1.0);
	for (f_it = faces_begin(); f_it != faces_end(); ++f_it)
	{
		for (fv_it = fv_iter(f_it); fv_it; ++fv_it)
		{
			//glNormal3dv(normal(fv_it.handle()));
			glVertex3dv(point(fv_it.handle()).data());
		}
	}
	glEnd();


	//glDisable(GL_POLYGON_OFFSET_FILL);


	glPushAttrib(GL_LIGHTING_BIT);
	glDisable(GL_LIGHTING);
	glLineWidth(1.0);
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINES);
	for (OMT::EIter e_it = edges_begin(); e_it != edges_end(); ++e_it)
	{
		OMT::HEHandle _hedge = halfedge_handle(e_it.handle(), 1);

		OMT::Point curVertex = point(from_vertex_handle(_hedge));
		glVertex3dv(curVertex.data());

		curVertex = point(to_vertex_handle(_hedge));
		glVertex3dv(curVertex.data());
	}
	glEnd();
	glPopAttrib();
	std::vector<double*>::iterator pt_it;
	if (pair.size() > 0)
	{
		for (int i = 0; i < 2; i++)
		{
			glPointSize(10.0f);
			glBegin(GL_POINTS);
			glColor3f(0, 1.0, 0);
			glVertex3dv(pair[i]);
			glEnd();
		}
	}
	if (pts.size() > 0)
	{
		for (pt_it = pts.begin(); pt_it != pts.end(); ++pt_it)
		{
			glPointSize(10.0f);
			glBegin(GL_POINTS);
			glColor3f(1.0, 0, 0);
			glVertex3dv(*pt_it);
			glEnd();
		}
	}
}

void Tri_Mesh::Render_Wireframe()
{
	//glPushAttrib(GL_LIGHTING_BIT);	
	glDisable(GL_LIGHTING);
	glLineWidth(1.0);

	glColor3f(0.0, 0.0, 0.0);

	glBegin(GL_LINES);
	for (OMT::EIter e_it = edges_begin(); e_it != edges_end(); ++e_it)
	{
		OMT::HEHandle _hedge = halfedge_handle(e_it.handle(), 1);

		OMT::Point curVertex = point(from_vertex_handle(_hedge));
		glVertex3dv(curVertex.data());

		curVertex = point(to_vertex_handle(_hedge));
		glVertex3dv(curVertex.data());
	}
	glEnd();

}


void Tri_Mesh::Render_Point()
{
	glPointSize(8.0);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POINTS);
	for (OMT::VIter v_it = vertices_begin(); v_it != vertices_end(); ++v_it)
	{
		glVertex3dv(point(v_it).data());
	}
	glEnd();
}

void Tri_Mesh::Model_Init_Property()
{
	this->add_property(QvHandle, QvName);
	this->property(QvHandle, vertices_begin()) = Eigen::Matrix4d::Zero();

	this->add_property(QeHandle, QeName);
	this->add_property(NewVertexHandle, NewVertexName);

}

void Tri_Mesh::simplification() 
{
	int count = 0;
	for (EIter eh = edges_begin(); eh != edges_end(); ++eh)
	{
		
		if (Checkangle(eh)) 
		{
		//	std::cout << "collaspe "<< std::endl;
			HalfedgeHandle ehalf = halfedge_handle(eh.handle(), 1);
			VHandle to = to_vertex_handle(ehalf);
			VHandle from = from_vertex_handle(ehalf);
			set_point(to, (point(to) + point(from)) / 2);
			collapse_edge(ehalf);
			garbage_collection();
			break;
		}
		//std::cout << std::endl;
	}
}
bool Tri_Mesh::Checkangle(EIter eh)
{
	int toid = 0, fromid = 0;
	std::vector<int> ring;
		pts.resize(0);
		points.resize(0);
		pair.resize(0);

		HalfedgeHandle ehalf = halfedge_handle(eh.handle(), 1);
		VHandle to = to_vertex_handle(ehalf);
		VHandle from = from_vertex_handle(ehalf);
		pair.push_back(point(to).data());
		pair.push_back(point(from).data());

		for (VVIter vvit = vv_begin(to); vvit; ++vvit)
		{
			if (vvit.handle().idx() != to.idx() && vvit.handle().idx() != from.idx())
			{
				ring.push_back(vvit.handle().idx());
				pts.push_back(point(vvit).data());
				points.push_back(point(vvit));
			}
		}

		for (VVIter vvit = vv_begin(from); vvit != vv_end(from); ++vvit)
		{
			if (vvit.handle().idx() != to.idx() && vvit.handle().idx() != from.idx())
			{
				std::vector<int>::iterator target = find(ring.begin(), ring.end(), vvit.handle().idx());

				if (target == ring.end())
				{
					ring.push_back(vvit.handle().idx());
					pts.push_back(point(vvit).data());
					points.push_back(point(vvit));
				}
			}
		}
		//std::cout << "point size " <<  points.size()<< std::endl;
		if(points.size() ==0)
		{
		//	std::cout << "Empty" << std::endl;
		}
		std::vector<Point>::iterator pts_it;
		double angle;
		for (pts_it = points.begin(); pts_it != points.end(); ++pts_it)
		{
			if (*pts_it == *points.begin()) 
			{
			//	std::cout << "begin" << std::endl;
				angle = PointAngle(*(points.end()-1 ), *(pts_it+1) , *(points.begin()) );
			}
			else if (*pts_it ==*(points.end()-1)) 
			{
			//	std::cout << "end" << std::endl;
				angle = PointAngle(*(pts_it - 1),*points.begin() , *(points.end()-1));
			}
			else 
			{
			
				angle = PointAngle(*(pts_it - 1), *(pts_it + 1),*pts_it);
			}
		//	std::cout << "Angle " << angle << std::endl;
			if (angle > 180) 
			{
				return false;
			}
		}


		return true;
}

void Tri_Mesh::ErrorQuadricsMatrix()
{
	
	for (VIter vh = vertices_begin(); vh != vertices_end(); ++vh)
	{
		Eigen::Matrix4d Qv = Eigen::Matrix4d::Zero();
		for (VFIter vfh = vf_begin(vh); vfh; ++vfh)
		{
			Normal fn = calc_face_normal(vfh);
			Point v = point(vh);
			Eigen::Vector4d q(fn[0], fn[1], fn[2], 0);
			q[3] = fn[0] * -v[0] + fn[1] * -v[1] + fn[2] * -v[2];
			Qv += q * q.transpose();
		}
		this->property(QvHandle, vh) = Qv;
	}
	

	for (EIter eh = edges_begin(); eh != edges_end(); ++eh)
	{
		HalfedgeHandle heh = halfedge_handle(eh, 0);
		VHandle toh = to_vertex_handle(heh);
		VHandle fromh = from_vertex_handle(heh);
		Eigen::Matrix4d Qbar = this->property(QvHandle, toh) + this->property(QvHandle, fromh);
		Eigen::Matrix4d qij = Qbar;
		Eigen::Vector4d B(0, 0, 0, 1);
		qij.row(3) = B;
		qij = qij.inverse().eval();
		Eigen::Vector4d NewVertex = qij.colPivHouseholderQr().solve(B);
		double Qe = NewVertex.transpose() * (Qbar * NewVertex);
		
		this->property(NewVertexHandle, eh) = NewVertex;
		this->property(QeHandle, eh) = Qe;
		ErrorPrority.push_back(std::pair<double, int>(Qe, eh.handle().idx()));
	}
	
	std::sort(std::begin(ErrorPrority), std::end(ErrorPrority), ErrorCompare);

	/*for (int i = 0; i < ErrorPrority.size(); i++)
	{
		std::cout << ErrorPrority[i].second << " " << ErrorPrority[i].first << "\n";
	}*/
}

void Tri_Mesh::KillEdge()
{
	//find First edge(victim) to kill ((for now))
	Tri_Mesh::EdgeIter VictimEI = this->edges_begin();

	//Use this edge (form) and (to) point
	Tri_Mesh::EHandle VictimEh = this->edge_handle(VictimEI->idx());
	Tri_Mesh::HalfedgeHandle VictimHeH = this->halfedge_handle(VictimEh, 0);
	Tri_Mesh::VertexHandle VictimFromVertexH = this->from_vertex_handle(VictimHeH);
	Tri_Mesh::VertexHandle VictimToVertexH = this->to_vertex_handle(VictimHeH);
	//
	Tri_Mesh::VertexHandle FirstPointFisrtcommon;
	Tri_Mesh::VertexHandle FirstPointsecondcommon;
	Tri_Mesh::VertexHandle secondPointFisrtcommon;
	Tri_Mesh::VertexHandle secondPointsecondcommon;

	//find two outside point 
	for (Tri_Mesh::VVIter VV = this->vv_begin(VictimFromVertexH); VV != this->vv_end(VictimFromVertexH); ++VV)
	{

	}
	for (Tri_Mesh::VVIter VV = this->vv_begin(VictimToVertexH); VV != this->vv_end(VictimToVertexH); ++VV)
	{

	}



}

void Tri_Mesh::Buffer()
{



}

void Tri_Mesh::GPURender()
{
}

void Tri_Mesh::CountdeltaE()
{
	//OpenMesh::EPropHandleT<Tri_Mesh::Point> Wi;


}

bool ErrorCompare(const std::pair<double, int>& a, const std::pair<double, int>& b)
{
	return a.first < b.first;
}
double PointAngle(Tri_Mesh::Point P1, Tri_Mesh::Point P2, Tri_Mesh::Point VPoint)
{
	double V1x = (P1[0] - VPoint[0]);
	double V1y = (P1[1] - VPoint[1]);
	double V1z = (P1[2] - VPoint[2]);
	double V2x = (P2[0] - VPoint[0]);
	double V2y = (P2[1] - VPoint[1]);
	double V2z = (P2[2] - VPoint[2]);
double A_B = V1x * V2x + V1y * V2y + V1z * V2z;
	double lAl = sqrt(V1x*V1x + V1y * V1y + V1z * V1z);
	double lBl = sqrt(V2x*V2x + V2y * V2y + V2z * V2z);

	double angle = acos(A_B / (lAl*lBl));

	angle = (angle*180.0) / 3.1415926;
	return angle;
}


void Tri_Mesh::LSMesh()
{

	Eigen::SparseMatrix<double> A(this->n_vertices() * 2, this->n_vertices());
	for (int i = 0; i < this->n_vertices(); i++)
	{
		A.insert(i, i) = 1.0;
		A.insert(i + this->n_vertices(), i) = 1.0;
	}
	Eigen::SparseQR<Eigen::SparseMatrix<double>, Eigen::COLAMDOrdering<int>> linearSolver;
	Eigen::VectorXd Bx(this->n_vertices());
	Eigen::VectorXd By(this->n_vertices());
	Eigen::VectorXd Bz(this->n_vertices());

	OpenMesh::EPropHandleT<Tri_Mesh::Point> Wi;
	this->add_property(Wi);
	//Do wi==================================================================================================================================
	for (EIter EI = this->edges_begin(); EI != this->edges_end(); ++EI)
	{
		Point tmepWi;

		EHandle Eh = this->edge_handle(EI->idx());
		HHandle HeH = this->halfedge_handle(Eh, 0);

		VHandle FromVertexH = from_vertex_handle(HeH);
		VHandle ToVertexH = to_vertex_handle(HeH);

		if (is_boundary(HeH))
			VHandle OppositeVertexH = this->opposite_vh(HeH);
		else return;
		if (is_boundary(opposite_halfedge_handle(HeH)))
			VHandle OppoOppoVertexH = this->opposite_he_opposite_vh(HeH);
		else return;
/*
		Point FromVertex = this->point(FromVertexH);
		Point ToVertex = this->point(ToVertexH);
		Point OppositeVertex = this->point(OppositeVertexH);
		Point OppoOppoVertex = this->point(OppoOppoVertexH);*/

//		double Angle1, Angle2;
//
//		Angle1 = PointAngle(FromVertex, ToVertex, OppositeVertex);
//		Angle2 = PointAngle(FromVertex, ToVertex, OppoOppoVertex);
//#ifdef DEBUG
//		std::cout << "Tri1_1" << FromVertex[0] << " " << FromVertex[1] << " " << FromVertex[2] << std::endl;
//		std::cout << "Tri1_2" << ToVertex[0] << " " << ToVertex[1] << " " << ToVertex[2] << std::endl;
//		std::cout << "Tri1_3" << OppositeVertex[0] << " " << OppositeVertex[1] << " " << OppositeVertex[2] << std::endl;
//		std::cout << "Tri2_3" << OppoOppoVertex[0] << " " << OppoOppoVertex[1] << " " << OppoOppoVertex[2] << std::endl;
//		std::cout << "Angles 1 2 " << Angle1 << " " << Angle2 << std::endl;
//#endif // DEBUG
//
//		tmepWi[0] = ((1.0 / tan((Angle1*3.1415926) / 180.0)) + (1.0 / tan((Angle2*3.1415926) / 180.0)));
//#ifdef DEBUG
//		std::cout << tmepWi[0] << std::endl;
//#endif // DEBUG
//		tmepWi[1] = 123;
//		tmepWi[2] = 456;
//		this->property(Wi, *EI) = tmepWi;

	}
//#ifdef DEBUG
//	std::cout << std::endl;
//#endif // DEBUG
//	for (int j = 0; j < this->n_vertices(); j++)
//	{
//		Bx[j] = 0;
//		By[j] = 0;
//		Bz[j] = 0;
//	}

//	//make matrix================================================================================================================
//	for (Tri_Mesh::VertexIter VI = this->vertices_begin(); VI != this->vertices_end(); ++VI)
//	{
//		float ABigWi = 0;
//		Tri_Mesh::VertexHandle NowVertex = this->vertex_handle(VI->idx());
//		std::vector<double> ThisA_Array;
//		for (int j = 0; j < this->n_vertices(); j++)
//		{
//			ThisA_Array.push_back(0.0);
//		}
//#ifdef DEBUG
//		cout << "NOW POINT" << InnerPoint[i] << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
//#endif // DEBUG
//
//		//Done Matrix===========================================================================================================================
//		for (Tri_Mesh::VVIter VV = this->vv_begin(NowVertex); VV != this->vv_end(NowVertex); ++VV)
//		{
//			float ThisVertexWi = 0;
//			Tri_Mesh::VertexHandle TargetVet = this->vertex_handle(VV->idx());
//
//
//			for (Tri_Mesh::VEIter VE = this->ve_begin(NowVertex); VE != this->ve_end(NowVertex); ++VE)
//			{
//				Tri_Mesh::EdgeHandle Eh = this->edge_handle(VE->idx());
//				Tri_Mesh::HalfedgeHandle HeH1 = this->halfedge_handle(Eh, 0);
//				Tri_Mesh::HalfedgeHandle HeH2 = this->halfedge_handle(Eh, 1);//?
//				Tri_Mesh::VertexHandle HeH1_FromVetx = this->from_vertex_handle(HeH1);
//				Tri_Mesh::VertexHandle HeH1_ToVetx = this->to_vertex_handle(HeH1);
//
//				if ((HeH1_FromVetx.idx() == TargetVet.idx()) && (HeH1_ToVetx.idx() == NowVertex.idx()))
//				{
//					Tri_Mesh::Point EdgeWi = this->property(Wi, Eh);
//					ThisVertexWi = EdgeWi[0];
//#ifdef DEBUG
//					cout << "wi Left " << ThisVertexWi << endl << endl;
//#endif // DEBUG
//					for (int j = 0; j < this->n_vertices(); j++)
//					{
//						if (VI->idx() == TargetVet.idx())
//						{
//							ThisA_Array[j] = ThisVertexWi;
//							break;
//						}
//					}
//					break;
//				}
//				else if ((HeH1_FromVetx.idx() == NowVertex.idx()) && (HeH1_ToVetx.idx() == TargetVet.idx()))
//				{
//					Tri_Mesh::Point EdgeWi = this->property(Wi, Eh);
//					ThisVertexWi = EdgeWi[0];
//#ifdef DEBUG
//					cout << "wi Left " << ThisVertexWi << endl << endl;
//#endif // DEBUG
//					for (int j = 0; j < this->n_vertices(); j++)
//					{
//						if (VI->idx() == TargetVet.idx())
//						{
//							ThisA_Array[j] = ThisVertexWi;
//							break;
//						}
//					}
//					break;
//				}
//
//			}
//			ABigWi += ThisVertexWi;
//		}
//		Bx[VI->idx()] = Bx[VI->idx()] / ABigWi;
//		By[VI->idx()] = By[VI->idx()] / ABigWi;
//		Bz[VI->idx()] = Bz[VI->idx()] / ABigWi;
//		for (int j = 0; j < this->n_vertices(); j++)
//		{
//			if (VI->idx() != j)
//			{
//				A.insert(VI->idx(), j) = ((-ThisA_Array[j]) / ABigWi);
//			}
//		}
//		std::cout << "Line " << VI->idx() << std::endl;
//	}
//#ifdef DEBUG
//	cout << "A" << endl;
//	cout << A;
//	cout << "Bx" << endl;
//	cout << Bx;
//	cout << "By" << endl;
//	cout << By;
//#endif // DEBUG
//	A.makeCompressed();
//	linearSolver.compute(A);
//	Eigen::VectorXd Xx = linearSolver.solve(Bx);
//	linearSolver.compute(A);
//	Eigen::VectorXd Xy = linearSolver.solve(By);
//	linearSolver.compute(A);
//	Eigen::VectorXd Xz = linearSolver.solve(Bz);
//	std::cout << "liner Solve!!" << std::endl;
//
//	for (Tri_Mesh::VIter VI = this->vertices_begin(); VI != this->vertices_end(); ++VI)
//	{
//		Tri_Mesh::VertexHandle VH = this->vertex_handle(VI->idx());
//		Tri_Mesh::Point VP = this->point(VH);
//		VP[0] = Xx[VI->idx()];
//		VP[1] = Xy[VI->idx()];
//		VP[2] = Xz[VI->idx()];
//	}

}


bool ReadFile(std::string _fileName, Tri_Mesh *_mesh)
{
	bool isRead = false;
	OpenMesh::IO::Options opt;
	if (OpenMesh::IO::read_mesh(*_mesh, _fileName, opt))
	{
		//read mesh from filename OK!
		isRead = true;
	}
	if (isRead)
	{
		// If the file did not provide vertex normals and mesh has vertex normal ,then calculate them
		if (!opt.check(OpenMesh::IO::Options::VertexNormal) && _mesh->has_vertex_normals())
		{
			_mesh->update_normals();
		}
	}
	return isRead;
}

bool SaveFile(std::string _fileName, Tri_Mesh *_mesh)
{
	bool isSave = false;
	if (OpenMesh::IO::write_mesh(*_mesh, _fileName))
	{
		isSave = true;
	}
	return isSave;
}
