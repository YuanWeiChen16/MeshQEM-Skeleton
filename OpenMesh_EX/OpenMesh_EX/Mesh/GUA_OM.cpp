#include "GUA_OM.h"
std::map <int, bool> deleteEdge;
//#define DEBUG
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
			OpenMesh::Vec3d p = point(fv_it.handle());
			p = p / scale;
			glVertex3dv(p.data());
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
		curVertex = curVertex / scale;
		glVertex3dv(curVertex.data());

		curVertex = point(to_vertex_handle(_hedge));
		curVertex = curVertex / scale;
		glVertex3dv(curVertex.data());
	}
	glEnd();
	//glPopAttrib();
	//glPointSize(100.0f);
	//glBegin(GL_POINTS);
	//glColor3f(1.0, 0, 0);
	//glVertex3d(0, 0, 0);
	//glEnd();
	/*std::vector<double*>::iterator pt_it;
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
	}*/
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
	this->add_property(InitIDHandle, InitIDName);
	this->add_property(QeHandle, QeName);
	this->add_property(NewVertexHandle, NewVertexName);
	this->add_property(this->Wi, WiName);
	this->add_property(this->WH, WHName);
	this->add_property(this->Ai, AiName);
	deleteEdge.clear();
	points.clear();
	pts.clear();
	pair.clear();
	Errorprority.clear();
}

bool Tri_Mesh::simplification(double& angleT, double& matrixT, double& vectorT)
{
	//std::cout << "face Count: " << n_faces() << "\n";
	int count = 0;
	int index = 0;
	EdgeHandle eh;
	bool colsw = false;
	Eigen::Vector4d etmpv;
	OpenMesh::Vec3d tmpv;
	//std::vector <std::pair<double, EHandle>>::iterator ep_iter;

	//for (ep_iter = ErrorPrority.begin(); ep_iter != ErrorPrority.end(); ++ep_iter)
	//for (auto ep_iter = Errorprority.begin(); ep_iter != Errorprority.end(); ++ep_iter)
	std::multiset <ErrorData>::iterator ep_iter = Errorprority.begin();
	while (ep_iter != Errorprority.end())
	{
		//eh = edge_handle(ep_iter->second.idx());
		if (status(ep_iter->ei.handle()).deleted()) {
			ep_iter = Errorprority.erase(ep_iter);
			continue;
		}
		eh = edge_handle(ep_iter->ei.handle().idx());
		etmpv = property(NewVertexHandle, eh);
		tmpv[0] = etmpv[0];
		tmpv[1] = etmpv[1];
		tmpv[2] = etmpv[2];
		clock_t start, end;
		start = clock();
		std::map <int, double> checkQe;
		if (Checkangle(eh))
		{
			end = clock();
			//std::cout << double(end - start) / CLOCKS_PER_SEC << "\n";
			HalfedgeHandle ehalf = halfedge_handle(eh, 1);
			VHandle to = to_vertex_handle(ehalf);
			VHandle from = from_vertex_handle(ehalf);
			Point tmpto = point(to);
			set_point(to, tmpv);
			colsw = is_collapse_ok(ehalf);
			end = clock();
			angleT = double(end - start) / CLOCKS_PER_SEC;
			//std::cout << double(end - start) / CLOCKS_PER_SEC << "\n";
			if (colsw)
			{
				collapse(ehalf);
				start = clock();
				//std::cout << double(end - start) / CLOCKS_PER_SEC << "\n";
				UpdateErrorMatrix(to, checkQe);
				end = clock();
				matrixT = double(end - start) / CLOCKS_PER_SEC;
				//std::cout << double(end - start) / CLOCKS_PER_SEC << "\n";
				//garbage_collection();
				start = clock();
				//std::cout << double(end - start) / CLOCKS_PER_SEC << "\n";
				UpdateErrorVector(checkQe, n_edges());
				end = clock();
				vectorT = double(end - start) / CLOCKS_PER_SEC;
				//std::cout << double(end - start) / CLOCKS_PER_SEC << "\n";
				//std::cout << "\n";
				return true;
			}
			else
			{
				set_point(to, tmpto);
				++ep_iter;
			}
		}
		else ++ep_iter;
	}
	return false;
}
bool Tri_Mesh::Checkangle(EdgeHandle eh)
{
	int toid = 0, fromid = 0;
	std::vector<int> ring;
	pts.clear();
	points.clear();
	pair.clear();

	HalfedgeHandle ehalf = halfedge_handle(eh, 1);
	VHandle to = to_vertex_handle(ehalf);
	VHandle from = from_vertex_handle(ehalf);
	pair.push_back(point(to).data());
	pair.push_back(point(from).data());
	for (VEIter vei = ve_begin(to); vei; ++vei)
	{
		if (status(vei).deleted()) continue;
		HalfedgeHandle vehalf = halfedge_handle(vei, 1);
		VHandle vto = to_vertex_handle(vehalf);
		VHandle vfrom = from_vertex_handle(vehalf);
		VHandle insertVertex = vto.idx() == to.idx() ? vfrom : vto;
		if (insertVertex.idx() == from.idx()) continue;
		points.push_back(point(insertVertex));
	}
	for (VEIter vei = ve_begin(from); vei; ++vei)
	{
		if (status(vei).deleted()) continue;
		HalfedgeHandle vehalf = halfedge_handle(vei, 1);
		VHandle vto = to_vertex_handle(vehalf);
		VHandle vfrom = from_vertex_handle(vehalf);
		VHandle insertVertex = vto.idx() == from.idx() ? vfrom : vto;
		if (insertVertex.idx() == to.idx()) continue;
		std::vector<int>::iterator target = find(ring.begin(), ring.end(), insertVertex.idx());

		if (target == ring.end())
			points.push_back(point(insertVertex));
	}


	//for (VVIter vvit = vv_begin(to); vvit; ++vvit)
	//{
	//	if (vvit.handle().idx() != to.idx() && vvit.handle().idx() != from.idx())
	//	{
	//		ring.push_back(vvit.handle().idx());
	//		pts.push_back(point(vvit).data());
	//		points.push_back(point(vvit));
	//	}
	//}

	//for (VVIter vvit = vv_begin(from); vvit != vv_end(from); ++vvit)
	//{
	//	if (vvit.handle().idx() != to.idx() && vvit.handle().idx() != from.idx())
	//	{
	//		std::vector<int>::iterator target = find(ring.begin(), ring.end(), vvit.handle().idx());

	//		if (target == ring.end())
	//		{
	//			ring.push_back(vvit.handle().idx());
	//			pts.push_back(point(vvit).data());
	//			points.push_back(point(vvit));
	//		}
	//	}
	//}
	//std::cout << "point size " <<  points.size()<< std::endl;
	if (points.size() == 0)
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
			angle = PointAngle(*(points.end() - 1), *(pts_it + 1), *(points.begin()));
		}
		else if (*pts_it == *(points.end() - 1))
		{
			//	std::cout << "end" << std::endl;
			angle = PointAngle(*(pts_it - 1), *points.begin(), *(points.end() - 1));
		}
		else
		{

			angle = PointAngle(*(pts_it - 1), *(pts_it + 1), *pts_it);
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
	std::map <int, Eigen::Vector4d> plane;
	for (VIter vh = vertices_begin(); vh != vertices_end(); ++vh)
	{
		cal_Qv(vh.handle(), plane);
	}

	//ErrorPrority.resize(n_edges());
	Errorprority.clear();
	for (EIter eh = edges_begin(); eh != edges_end(); ++eh)
	{
		this->property(InitIDHandle, eh) = eh.handle().idx();
		double Qe = cal_Qe(eh.handle());
		Errorprority.insert(ErrorData(Qe, eh, this->property(InitIDHandle, eh)));
		//ErrorPrority[eh.handle().idx()] = (std::pair<double, EdgeHandle>(Qe, eh.handle()));
	}

	//std::sort(std::begin(ErrorPrority), std::end(ErrorPrority), ErrorCompare);

	/*for (int i = 0; i < ErrorPrority.size(); i++)
	{
		std::cout << ErrorPrority[i].second << " " << ErrorPrority[i].first << "\n";
	}*/
}

void Tri_Mesh::UpdateErrorMatrix(VertexHandle vh, std::map <int, double>& checkQe)
{
	//Errorprority.clear();
	std::map<int, Eigen::Vector4d> plane;
	cal_Qv(vh, plane);

	for (VVIter vvi = vv_begin(vh); vvi; ++vvi)
	{
		if (status(vvi).deleted()) continue;
		cal_Qv(vvi.handle(), plane);
	}

	//std::map <int, bool> checkQe;
	for (VEIter vei = ve_begin(vh); vei; ++vei)
	{
		if (status(vei).deleted()) {
			deleteEdge[vei.handle().idx()] = true;
			continue;
		}
		cal_Qe(vei.handle());
		checkQe[this->property(InitIDHandle, vei)] = this->property(QeHandle, vei);
	}

	for (VVIter vvi = vv_begin(vh); vvi; ++vvi)
	{
		for (VEIter vei = ve_begin(vvi); vei; ++vei)
		{
			if (status(vei).deleted() || (checkQe.find(vei.handle().idx()) != checkQe.end()))
			{
				deleteEdge[vei.handle().idx()] = true;
				continue;
			}
			cal_Qe(vei.handle());
			checkQe[this->property(InitIDHandle, vei)] = this->property(QeHandle, vei);
		}
	}
}

void Tri_Mesh::UpdateErrorVector(std::map <int, double>& checkQe, int edgeSize)
{
	//ErrorPrority.clear();
	//std::vector<std::pair<double, EHandle>>().swap(ErrorPrority);
	//ErrorPrority.resize(n_edges());

	std::multiset <ErrorData>::iterator it = Errorprority.begin();
	std::vector<ErrorData> insertData;
	//insertData.resize(checkQe.size());
	int n = 0;
	int offset = 0;

	while (it != Errorprority.end())
	{
		int id = it->ei.handle().idx();
		if (is_valid_handle(it->ei) && !status(it->ei).deleted())
		{

			if (checkQe.find(it->InitID) != checkQe.end())
			{

				double qe = this->property(QeHandle, edge_handle(id));

				insertData.push_back(ErrorData(this->property(QeHandle, it->ei), it->ei, this->property(InitIDHandle, it->ei)));

				it = Errorprority.erase(it);
			}
			else
			{
				if (this->property(QeHandle, it->ei) != it->Qe)
				{
					//insertData.resize(offset + 1 + checkQe.size());
					insertData.push_back(ErrorData(this->property(QeHandle, it->ei), it->ei, this->property(InitIDHandle, it->ei)));
					it = Errorprority.erase(it);
				}
				else ++it;

			}
		}
		else
		{
			it = Errorprority.erase(it);

		}
	}
	for (int i = 0; i < insertData.size(); i++)
	{
		Errorprority.insert(insertData[i]);
	}

	//Errorprority.clear();
	//for (EIter ei = edges_begin(); ei != edges_end(); ++ei)
	//{
	//	//if (status(ei).deleted()) ErrorPrority[ei.handle().idx()] = (std::pair<double, EdgeHandle>(DBL_MAX, ei));
	//	//else ErrorPrority[ei.handle().idx()] = (std::pair<double, EdgeHandle>(this->property(QeHandle, ei), ei));
	//	Errorprority.insert(ErrorData(this->property(QeHandle, ei), ei, this->property(InitIDHandle, ei)));

	//}
	//std::sort(ErrorPrority.begin(), ErrorPrority.end(), ErrorCompare);
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

void Tri_Mesh::testBox()
{
	std::vector <EdgeIter> edgeVector;

	//std::cout << n_vertices() << " " << n_edges() << "\n";
	//std::cout << "\n\nVector\n";
	for (std::multiset <ErrorData>::iterator it = Errorprority.begin(); it != Errorprority.end(); ++it)
	{
		if (!is_valid_handle((*it).ei))
			std::cout << "Invalid Edge!\n";
		else
		{
			EdgeIter ei = (*it).ei;
			HHandle heh = halfedge_handle(ei, 0);
			VHandle tov = to_vertex_handle(heh);
			VHandle fromv = from_vertex_handle(heh);
			std::cout << "Edge index: " << ei.handle().idx() << "\t" << fromv.idx() << "\t" << tov.idx() << "\t" << it->Qe << "\n";
		}
	}
	//EdgeHandle eh = edge_handle(ErrorPrority[0].second.idx());

	edgeVector.resize(n_edges());
	for (EIter ei = edges_begin(); ei != edges_end(); ++ei)
	{
		edgeVector[ei.handle().idx()] = ei;
		HHandle heh = halfedge_handle(ei, 0);
		VHandle tov = to_vertex_handle(heh);
		VHandle fromv = from_vertex_handle(heh);
		std::cout << "Edge index: " << ei.handle().idx() << "\t" << fromv.idx() << "\t" << tov.idx() << "\n";
		this->property(InitIDHandle, ei) = ei.handle().idx();
	}
	for (int i = 0; i < edgeVector.size(); i++)
	{
		HHandle heh = halfedge_handle(edgeVector[i], 0);
		VHandle tov = to_vertex_handle(heh);
		VHandle fromv = from_vertex_handle(heh);
		std::cout << "edge ID: " << (edgeVector[i]).handle().idx() << "\t from: " << fromv.idx() << "\t to: " << tov.idx() << "\n";

	}
	HHandle eheh = halfedge_handle(edges_begin(), 0);
	VHandle htov = to_vertex_handle(eheh);
	VHandle hfromv = from_vertex_handle(eheh);
	std::cout << "collapse edge: " << 0 << "\t" << htov.idx() << "\t" << hfromv.idx() << "\n";
	std::map <int, double> checkQe;
	collapse(halfedge_handle(edges_begin(), 0));
	std::cout << n_vertices() << " " << n_edges() << "\n";
	UpdateErrorMatrix(htov, checkQe);
	garbage_collection();
	UpdateErrorVector(checkQe, n_edges());

	for (EIter ei = edges_begin(); ei != edges_end(); ++ei)
	{
		HHandle heh = halfedge_handle(ei, 0);
		VHandle tov = to_vertex_handle(heh);
		VHandle fromv = from_vertex_handle(heh);
		std::cout << "Edge index: " << ei.handle().idx() << "\t" << fromv.idx() << "\t" << tov.idx() << "\t" << this->property(InitIDHandle, ei) << "\n";
	}
	std::cout << "\n\n";
	for (std::multiset <ErrorData>::iterator it = Errorprority.begin(); it != Errorprority.end(); ++it)
	{
		if (!is_valid_handle(it->ei))
			std::cout << "Invalid Edge! " << it->ei.handle().idx() << "\n";
		else
		{
			EdgeIter ei = it->ei;
			HHandle heh = halfedge_handle(ei, 0);
			VHandle tov = to_vertex_handle(heh);
			VHandle fromv = from_vertex_handle(heh);
			std::cout << "Edge index: " << ei.handle().idx() << "\t" << fromv.idx() << "\t" << tov.idx() << "\t" << it->Qe << "\n";
		}
	}
	/*std::cout << "\n\nVector\n";
	for (int i = 0; i < ErrorPrority.size(); i++)
	{
		std::cout << ErrorPrority[i].second.idx() << "\n";
	}
	std::cout << n_vertices() << " " << n_edges() << " " << "\n";*/
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

void Tri_Mesh::simplify()
{

}

bool ErrorCompare(const std::pair<double, Tri_Mesh::EdgeHandle>& a, const std::pair<double, Tri_Mesh::EdgeHandle>& b)
{
	return a.first < b.first;
}
int returnEdgeSize()
{
	return deleteEdge.size();
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
	double lAl = sqrt(V1x * V1x + V1y * V1y + V1z * V1z);
	double lBl = sqrt(V2x * V2x + V2y * V2y + V2z * V2z);
	double angle = acos(A_B / (lAl*lBl));
	return angle;
}

void Tri_Mesh::LSMesh(int t)
{
	double SL = 2.0;
	//Matrix!!!
	Eigen::SparseMatrix<double> L(this->n_vertices() * 2, this->n_vertices());
	Eigen::SimplicialLDLT<Eigen::SparseMatrix<double>> linearSolver;
	Eigen::VectorXd Bx(this->n_vertices() * 2);
	Eigen::VectorXd By(this->n_vertices() * 2);
	Eigen::VectorXd Bz(this->n_vertices() * 2);
	L.setZero();
	Bx.setZero();
	By.setZero();
	Bz.setZero();
	//Do wi==================================================================================================================================
	MakeLwi();
	//Do A0==================================================================================================================================

	if (t == 0)
	{
		MakeAreai();
		//total A(only first)
		double totalArea = this->MaketotalArea();
		this->OutsideWL = this->InitEH0 *totalArea;
		//WL = 5.28928;// 10*totalArea;
		//std::cout << totalArea << std::endl;
	}
	else
	{
		this->OutsideWL = this->OutsideWL * this->InitSL;
	}
	int persent = 0;
	//make matrix================================================================================================================
	for (Tri_Mesh::VertexIter VI = this->vertices_begin(); VI != this->vertices_end(); ++VI)
	{
		double ABigWi = 0;
		double A0 = this->property(Ai, VI);
		double At = 0;
		Tri_Mesh::VertexHandle NowVertex = this->vertex_handle(VI->idx());
		std::vector<double> ThisA_Array;
		std::vector<int> ThisA_Array_List;
		std::vector<Tri_Mesh::Point> tempPoint;
		//std::cout << "Line " << VI->idx() << "  ";
		//Done Matrix every row===========================================================================================================================
		for (Tri_Mesh::VVIter VV = this->vv_begin(NowVertex); VV != this->vv_end(NowVertex); ++VV)
		{
			double ThisVertexWi = 0;
			Tri_Mesh::VertexHandle TargetVet = this->vertex_handle(VV->idx());
			Tri_Mesh::HalfedgeHandle myedge = this->find_halfedge(NowVertex, VV);
			ThisA_Array.push_back(this->property(this->Wi, myedge));
			ThisA_Array_List.push_back(VV->idx());
			ABigWi += this->property(this->Wi, myedge);
			tempPoint.push_back(this->point(*VV));
		}
		Tri_Mesh::Point  midPoint = this->point(*VI);
		for (int i = 0; i < ThisA_Array_List.size(); i++)
		{
			//L.insert(VI->idx(), ThisA_Array_List[i]) = ((-ThisA_Array[i]) / ABigWi)*WL;
			L.insert(VI->idx(), ThisA_Array_List[i]) = (ThisA_Array[i])*this->OutsideWL;
			//std::cout << (ThisA_Array[i])*WL << "  ";
			//Heron's Formula
			double a = (midPoint - tempPoint[(i) % tempPoint.size()]).length();
			double b = (midPoint - tempPoint[(i + 1) % tempPoint.size()]).length();
			double c = (tempPoint[(i) % tempPoint.size()] - tempPoint[(i + 1) % tempPoint.size()]).length();
			double S = (a + b + c) / 2.0;
			At += std::sqrtf(S*(S - a)*(S - b)*(S - c));
		}


		if (At < 0.000001)
		{
			At = A0;
		}



		double Whi = 0;
		if (t == 0)
		{
			Whi = 1.0;
		}
		else
		{
			Whi = std::sqrtf(A0 / At);
		}
		int j = VI->idx();
		//init B
		L.insert(j, j) = (-1) * ABigWi * this->OutsideWL;
		//std::cout << "total wight " << (-1) * ABigWi * WL << std::endl;
		L.insert(j + this->n_vertices(), j) = 1.0*Whi;

		Bx[j + this->n_vertices()] = this->point(*VI)[0] * Whi;
		By[j + this->n_vertices()] = this->point(*VI)[1] * Whi;
		Bz[j + this->n_vertices()] = this->point(*VI)[2] * Whi;


		if (VI->idx() % (this->n_vertices() / 10) == 0)
		{

			std::cout<< persent <<"%"<<std::endl;
			persent += 10;
		}

		//std::cout << "vertex X " << Bx[j + this->n_vertices()] << " Y " << By[j + this->n_vertices()] << " Z " << Bz[j + this->n_vertices()] << std::endl;
	}

#ifdef DEBUG
	cout << "A" << endl;
	cout << A;
	cout << "Bx" << endl;
	cout << Bx;
	cout << "By" << endl;
	cout << By;
#endif // DEBUG

	Bx = L.transpose()*Bx;
	By = L.transpose()*By;
	Bz = L.transpose()*Bz;
	L = L.transpose()*L;

	L.makeCompressed();

	linearSolver.compute(L);

	Eigen::VectorXd Xx = linearSolver.solve(Bx);
	//linearSolver.compute(L);
	Eigen::VectorXd Xy = linearSolver.solve(By);
	//linearSolver.compute(L);
	Eigen::VectorXd Xz = linearSolver.solve(Bz);
	//std::cout << "liner Solve!!" << std::endl;

	for (Tri_Mesh::VIter VI = this->vertices_begin(); VI != this->vertices_end(); ++VI)
	{
		Tri_Mesh::VertexHandle VH = this->vertex_handle(VI->idx());
		this->point(VH)[0] = Xx[VI->idx()];
		this->point(VH)[1] = Xy[VI->idx()];
		this->point(VH)[2] = Xz[VI->idx()];		
	}

	this->request_face_normals();
	this->update_normals();
	this->release_face_normals();

	std::cout << "this is " << this->t << " LSMESH!!!!" << std::endl;

}

void Tri_Mesh::MakeLwi()
{
	for (Tri_Mesh::EIter EI = this->edges_begin(); EI != this->edges_end(); ++EI)
	{
		double tmepWi;

		EHandle Eh = this->edge_handle(EI->idx());
		HHandle HeH = this->halfedge_handle(Eh, 0);
		HHandle HeH2 = this->halfedge_handle(Eh, 1);

		VHandle FromVertexH = from_vertex_handle(HeH);
		VHandle ToVertexH = to_vertex_handle(HeH);
		VHandle OppositeVertexH;
		VHandle OppoOppoVertexH;
		if (is_boundary(HeH) == false)
			OppositeVertexH = this->opposite_vh(HeH);
		else return;
		if (is_boundary(opposite_halfedge_handle(HeH)) == false)
			OppoOppoVertexH = this->opposite_he_opposite_vh(HeH);
		else return;

		Point FromVertex = this->point(FromVertexH);
		Point ToVertex = this->point(ToVertexH);
		Point OppositeVertex = this->point(OppositeVertexH);
		Point OppoOppoVertex = this->point(OppoOppoVertexH);

		double Angle1, Angle2;
		Angle1 = PointAngle(FromVertex, ToVertex, OppositeVertex);
		Angle2 = PointAngle(FromVertex, ToVertex, OppoOppoVertex);
		tmepWi = (1.0 / tan(Angle1)) + (1.0 / tan(Angle2));
		if (tmepWi > 1000000000)
		{
			tmepWi = 1000000000;
		}
		if (tmepWi < -1000000000)
		{
			tmepWi = -1000000000;
		}
#ifdef DEBUG
		std::cout << EI->idx() << std::endl;
#endif // DEBUG
		this->property(this->Wi, HeH) = tmepWi;
		this->property(this->Wi, HeH2) = tmepWi;
	}
}

void Tri_Mesh::MakeAreai()
{
	for (Tri_Mesh::VertexIter VI = this->vertices_begin(); VI != this->vertices_end(); ++VI)
	{
		std::vector<Tri_Mesh::Point> tempPoint;
		for (Tri_Mesh::VVIter VV = this->vv_begin(*VI); VV != this->vv_end(*VI); ++VV)
		{
			tempPoint.push_back(this->point(*VV));
		}
		Tri_Mesh::Point  midPoint = this->point(*VI);
		double OneRingArea = 0;
		for (int i = 0; i < tempPoint.size(); i++)
		{
			//Heron's Formula
			double a = (midPoint - tempPoint[(i) % tempPoint.size()]).length();
			double b = (midPoint - tempPoint[(i + 1) % tempPoint.size()]).length();
			double c = (tempPoint[(i) % tempPoint.size()] - tempPoint[(i + 1) % tempPoint.size()]).length();
			double S = (a + b + c) / 2.0;
			OneRingArea += std::sqrtf(S*(S - a)*(S - b)*(S - c));
		}
		this->property(this->Ai, VI) = OneRingArea;
	}
}

double Tri_Mesh::MaketotalArea()
{
	//total A(only first)
	double totalArea = 0;
	for (FaceIter FI = this->faces_begin(); FI != this->faces_end(); ++FI)
	{
		std::vector<Point> tempPoint;
		for (FVIter FVI = this->fv_begin(FI); FVI != this->fv_end(FI); ++FVI)
		{
			tempPoint.push_back(this->point(FVI));
		}
		//Heron's Formula
		double a = (tempPoint[0] - tempPoint[1]).length();
		double b = (tempPoint[0] - tempPoint[2]).length();
		double c = (tempPoint[1] - tempPoint[2]).length();
		double S = (a + b + c) / 2.0;
		totalArea += std::sqrtf(S*(S - a)*(S - b)*(S - c));
	}
	return totalArea / this->n_faces();
}

bool Tri_Mesh::saveFile()
{
	std::fstream obj;
	obj.open("newModel.obj", std::ios::out);
	if (!obj)
	{
		std::cout << "!Can't open " << "newModel" << std::endl;
		return false;
	}

	/*for (int i = 0; i < Allmeshs.size(); i++)
	{
		Tri_Mesh temp = Allmeshs[i];
		obj << "# face " << i << std::endl;
		for (VIter viter = temp.vertices_begin(); viter != temp.vertices_end(); viter++) {
			double* p = new double[3];
			p = temp.point(viter.handle()).data();
			std::cout << "Index: " << viter.handle().idx() << " " << p[0] << " " << p[1] << " " << p[2] << std::endl;
			obj << "v " << p[0] << " " << p[1] << " " << p[2] << std::endl;
		}
		for (FIter fiter = temp.faces_begin(); fiter != temp.faces_end(); fiter++)
		{
			std::cout << "Index: " << fiter.handle().idx();
			obj << "f";
			for (FVIter fviter = temp.fv_iter(fiter); fviter; fviter++) {
				std::cout << " " << fviter.handle().idx() + 1;
				obj << " " << fviter.handle().idx() + 1;
			}
			std::cout << std::endl;
			obj << std::endl;
		}
	}*/


	for (VIter viter = vertices_begin(); viter != vertices_end(); viter++) {
		double* p = new double[3];
		p = point(viter.handle()).data();
		//std::cout << "Index: " << viter.handle().idx() << " " << p[0] << " " << p[1] << " " << p[2] << std::endl;
		obj << "v " << p[0] << " " << p[1] << " " << p[2] << std::endl;
	}
	for (FIter fiter = faces_begin(); fiter != faces_end(); fiter++)
	{
		//std::cout << "Index: " << fiter.handle().idx();
		obj << "f";
		for (FVIter fviter = fv_iter(fiter); fviter; fviter++) {
			//std::cout << " " << fviter.handle().idx() + 1 + offsets[i];
			obj << " " << fviter.handle().idx() + 1;
		}
		//std::cout << std::endl;
		obj << std::endl;
	}
	obj.close();
	return true;
}

int Tri_Mesh::faceSize()
{
	int count = 0;
	for (FIter fit = faces_begin(); fit != faces_end(); ++fit)
	{
		if (!status(fit).deleted())count++;
	}
	return count;
}

void Tri_Mesh::cal_Qv(VertexHandle vh, std::map<int, Eigen::Vector4d>& plane)
{
	Eigen::Matrix4d Qv = Eigen::Matrix4d::Zero();
	for (VFIter vfh = vf_begin(vh); vfh; ++vfh)
	{
		if (status(vfh).deleted()) continue;
		int id = vfh.handle().idx();
		if (plane.find(id) != plane.end())
			Qv = plane[id] * plane[id].transpose();
		else {
			Normal fn = calc_face_normal(vfh);
			Point v = point(vh);
			Eigen::Vector4d q(fn[0], fn[1], fn[2], 0);
			q[3] = fn[0] * -v[0] + fn[1] * -v[1] + fn[2] * -v[2];
			Qv += q * q.transpose();
		}
	}
	this->property(QvHandle, vh) = Qv;
}

double Tri_Mesh::cal_Qe(EdgeHandle eh)
{
	HalfedgeHandle heh = halfedge_handle(eh, 0);
	VHandle toh = to_vertex_handle(heh);
	VHandle fromh = from_vertex_handle(heh);
	Eigen::Matrix4d Qbar = this->property(QvHandle, toh) + this->property(QvHandle, fromh);
	Eigen::Matrix4d qij = Qbar;
	Eigen::Vector4d B(0, 0, 0, 1);
	qij.row(3) = B;
	//qij = qij.inverse().eval();
	Eigen::Vector4d NewVertex = qij.partialPivLu().solve(B);
	double Qe = NewVertex.transpose() * (Qbar * NewVertex);

	this->property(NewVertexHandle, eh) = NewVertex;
	this->property(QeHandle, eh) = Qe;
	return Qe;
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
