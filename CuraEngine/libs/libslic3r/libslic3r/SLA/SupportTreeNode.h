#ifndef SLIC3R_SUPPORTTREENODE_1637567374311_H
#define SLIC3R_SUPPORTTREENODE_1637567374311_H
#include <libslic3r/SLA/SupportTreeHeader.h>
#include <libslic3r/TriangleMesh.hpp>

namespace Slic3r
{
	namespace sla
	{
        /**
         * Terminology:
         *
         * Support point:
         * The point on the model surface that needs support.
         *
         * Pillar:
         * A thick column that spans from a support point to the ground and has
         * a thick cone shaped base where it touches the ground.
         *
         * Ground facing support point:
         * A support point that can be directly connected with the ground with a pillar
         * that does not collide or cut through the model.
         *
         * Non ground facing support point:
         * A support point that cannot be directly connected with the ground (only with
         * the model surface).
         *
         * Head:
         * The pinhead that connects to the model surface with the sharp end end
         * to a pillar or bridge stick with the dull end.
         *
         * Headless support point:
         * A support point on the model surface for which there is not enough place for
         * the head. It is either in a hole or there is some barrier that would collide
         * with the head geometry. The headless support point can be ground facing and
         * non ground facing as well.
         *
         * Bridge:
         * A stick that connects two pillars or a head with a pillar.
         *
         * Junction:
         * A small ball in the intersection of two or more sticks (pillar, bridge, ...)
         *
         * CompactBridge:
         * A bridge that connects a headless support point with the model surface or a
         * nearby pillar.
         */

        struct SupportTreeNode
        {
            enum  EventType
            {
                eventBase = 0x01,
                eventCreate = eventBase << 1,
                eventDelete = eventBase << 2,
                eventMax = eventBase << 3
            };
            typedef enum  Link_Dir
            {
                LEFT,
                RIGHT
            }LinkDir;
            static const constexpr long ID_UNSET = -1;

            long id = ID_UNSET; // For identification withing a tree.
            int eventFlg = SupportTreeNode::EventType::eventCreate;// the flg to mark whether this obj has been created
            MeshTypeElement treeNodeType = MeshTypeElement::BaseType;
            std::vector<SupportTreeNode2MeshLink>  linkL;//left link
            std::vector<SupportTreeNode2MeshLink>  linkR;//right link
            std::vector<SupportTreeNode2MeshLink>  briStartplink;//start point link right;

            inline void invalidate() {
                eventFlg = eventFlg | SupportTreeNode::eventDelete;
                linkL.clear();
                linkR.clear();
                briStartplink.size();
            }
            inline bool is_valid() const { return ((id >= 0) && ((eventFlg & SupportTreeNode::eventDelete) != SupportTreeNode::eventDelete)); }
            inline bool is_created() const { return (eventFlg & SupportTreeNode::eventCreate) == SupportTreeNode::eventCreate; }
            inline void setCreatedFlg(bool flg)
            {
                if (flg)
                    eventFlg = eventFlg | SupportTreeNode::eventCreate;//true: this obj has been created already
                else
                    eventFlg = eventFlg & ~SupportTreeNode::eventCreate;//false :this obj need been created 

            }
            inline void reConstructe(const SupportTreeNode& treenode)
            {
                eventFlg = eventFlg & ~SupportTreeNode::eventDelete;
                linkL = treenode.linkL;
                linkR = treenode.linkR;
                briStartplink = treenode.briStartplink;

                setCreatedFlg(true);
            }
            inline void setbriStartplink(SupportTreeNode linkNode)
            {
                SupportTreeNode2MeshLink temp;
                if (!is_valid() || !linkNode.is_valid())
                    return;
                temp.type = linkNode.treeNodeType;
                temp.id = linkNode.id;
                briStartplink.emplace_back(temp);
                if (briStartplink.size() != 1)
                    std::cout << "briStartplink size==" << briStartplink.size() << std::endl;
                //assert(briStartplink.size() == 1);
            }
            //inline void buildInnerlinkLeft( SupportTreeNode* obj)
            //{
            //    SupportTreeNode2MeshLink temp;
            //    temp.id = obj->id;
            //    temp.type = obj->treeNodeType;
            //    linkL.emplace_back(temp);
            //}
            //inline void buildInnerlinkRight( SupportTreeNode* obj)
            //{
            //    SupportTreeNode2MeshLink temp;
            //    temp.id = obj->id;
            //    temp.type = obj->treeNodeType;
            //    linkR.emplace_back(temp);
            //}

            //inline void buildInnerlink(SupportTreeNode* obj, LinkDir dir)
            //{
            //    assert(obj->treeNodeType != MeshTypeElement::BaseType);

            //    SupportTreeNode2MeshLink srclink;
            //    srclink.id = obj->id;
            //    srclink.type = obj->treeNodeType;
            //    switch (dir)
            //    {
            //    case LinkDir::LEFT:
            //    {
            //        SupportTreeNode2MeshLink temp;
            //        temp.type = treeNodeType;
            //        temp.id = id;
            //        obj->linkR.emplace_back(temp);
            //        linkL.emplace_back(srclink);
            //    }
            //    break;
            //    case LinkDir::RIGHT:
            //    {
            //        SupportTreeNode2MeshLink temp;
            //        temp.type = treeNodeType;
            //        temp.id = id;
            //        obj->linkL.emplace_back(temp);
            //        linkR.emplace_back(srclink);

            //    }
            //    break;
            //    default:
            //        break;
            //    }
            //}
        };

        // A pinhead originating from a support point
        struct Head : public SupportTreeNode {
            Vec3d dir = DOWN;
            Vec3d pos = { 0, 0, 0 };

            double r_back_mm = 1;
            double r_pin_mm = 0.5;
            double width_mm = 2;
            double penetration_mm = 0.5;

            double r_toptouch_mm = 0.2;//0.5
            bool   toptouch_enable = false;
            IndexedMesh::hit_result hit;
            // If there is a pillar connecting to this head, then the id will be set.
            long pillar_id = ID_UNSET;

            long bridge_id = ID_UNSET;
            int  postype = -1;
            Head(double r_big_mm,
                double r_small_mm,
                double length_mm,
                double penetration,
                const Vec3d& direction = DOWN,  // direction (normal to the dull end)
                const Vec3d& offset = { 0, 0, 0 }      // displacement
            );
            inline Head()
            {
                treeNodeType = MeshTypeElement::Head;
                id = ID_UNSET;
            }

            inline double real_width() const
            {
                return 2 * r_pin_mm + width_mm + 2 * r_back_mm;
            }

            inline double fullwidth() const
            {
                return real_width() - penetration_mm;
            }

            inline Vec3d junction_point() const
            {
                return pos + (fullwidth() - r_back_mm) * dir;
            }

            inline double request_pillar_radius(double radius) const
            {
                const double rmax = r_back_mm;
                return radius > 0 && radius < rmax ? radius : rmax;
            }
            inline void setToptouchRadius(double r_mm, bool enableflg)
            {
                r_toptouch_mm = r_mm;
                toptouch_enable = enableflg;
            }

        };

        // A junction connecting bridges and pillars
        struct Junction : public SupportTreeNode {
            double r = 1;
            Vec3d pos;
            Junction(const Vec3d& tr, double r_mm) : r(r_mm), pos(tr) { treeNodeType = MeshTypeElement::Junction; }
            Junction()
            {
                treeNodeType = MeshTypeElement::Junction;
                id = ID_UNSET;

            }


        };



        // A base for pillars or bridges that end on the ground
        struct Pedestal : public SupportTreeNode {
            Vec3d pos;
            double height, r_bottom, r_top;
            long pillar_id = ID_UNSET;

            Pedestal(const Vec3d& p, double h, double rbottom, double rtop)
                : pos{ p }, height{ h }, r_bottom{ rbottom }, r_top{ rtop }
            {treeNodeType = MeshTypeElement::Pedestal; }
            Pedestal()
            {
                treeNodeType = MeshTypeElement::Pedestal;
                id = ID_UNSET;

            }
        };

        // This is the thing that anchors a pillar or bridge to the model body.
        // It is actually a reverse pinhead.
        //struct Anchor: public Head { using Head::Head; };
        struct Anchor : public SupportTreeNode {

            Vec3d dir = DOWN;
            Vec3d pos = { 0, 0, 0 };

            double r_back_mm = 1.0;
            double r_pin_mm = 0.1;//0.5
            double width_mm = 2;
            double penetration_mm = 0.5;


            // If there is a pillar connecting to this head, then the id will be set.
            long pillar_id = ID_UNSET;

            long bridge_id = ID_UNSET;
            Anchor(double r_big_mm,
                double r_small_mm,
                double length_mm,
                double penetration,
                const Vec3d& direction = DOWN,  // direction (normal to the dull end)
                const Vec3d& offset = { 0, 0, 0 }      // displacement
            );
            Anchor()
            {
                treeNodeType = MeshTypeElement::Anchor;
                id = ID_UNSET;

            }
            inline double real_width() const
            {
                return 2 * r_pin_mm + width_mm + 2 * r_back_mm;
            }

            inline double fullwidth() const
            {
                return real_width() - penetration_mm;
            }

            inline Vec3d junction_point() const
            {
                return pos + (fullwidth() - r_back_mm) * dir;
            }

            inline double request_pillar_radius(double radius) const
            {
                const double rmax = r_back_mm;
                return radius > 0 && radius < rmax ? radius : rmax;
            }

        };
        // A Bridge between two pillars (with junction endpoints)
        struct Bridge : public SupportTreeNode {
            double r = 0.8;
            Vec3d startp = Vec3d::Zero(), endp = Vec3d::Zero();
            bool starts_from_head = false; // Could start from a junction as well
            long start_junction_id = ID_UNSET;
            Bridge(const Vec3d& j1,
                const Vec3d& j2,
                double       r_mm = 0.8) : r{ r_mm }, startp{ j1 }, endp{ j2 }
            {treeNodeType = MeshTypeElement::Bridge; }
            Bridge()
            {
                treeNodeType = MeshTypeElement::Bridge;
                id = ID_UNSET;

            }

            double get_length() const { return (endp - startp).norm(); }
            Vec3d  get_dir() const { return (endp - startp).normalized(); }


        };
        struct CrossBridge : public SupportTreeNode {
            double r = 0.8;
            Vec3d startp = Vec3d::Zero(), endp = Vec3d::Zero();
            bool starts_from_head = false; // Could start from a junction as well
            long start_junction_id = ID_UNSET;

            CrossBridge(const Vec3d& j1,
                const Vec3d& j2,
                double       r_mm = 0.8) : r{ r_mm }, startp{ j1 }, endp{ j2 }
            {treeNodeType = MeshTypeElement::CrossBridge; }
            CrossBridge()
            {
                treeNodeType = MeshTypeElement::CrossBridge;
                id = ID_UNSET;

            }
            double get_length() const { return (endp - startp).norm(); }
            Vec3d  get_dir() const { return (endp - startp).normalized(); }

        };

        struct DiffBridge : public Bridge {
            double end_r;

            DiffBridge(const Vec3d& p_s, const Vec3d& p_e, double r_s, double r_e)
                : Bridge{ p_s, p_e, r_s }, end_r{ r_e }
            {treeNodeType = MeshTypeElement::DiffBridge; }
            DiffBridge()
            {
                treeNodeType = MeshTypeElement::DiffBridge;
                id = ID_UNSET;

            }
        };

        struct Pillar : public SupportTreeNode {
            double height, r;
            Vec3d endpt;

            // If the pillar connects to a head, this is the id of that head
            bool starts_from_head = true; // Could start from a junction as well
            long start_junction_id = ID_UNSET;
            bool bridgePillar = false;
            int selfType = 0;
            // How many bridges are connected to this pillar
            unsigned bridges = 0;
            //std::vector<long>  m_linkBridges;
            //std::vector<long>  m_linkCrossBridges;
            //std::vector<long>  m_linkDiffBridges;
            //std::vector<long>  m_linkPedestals;//ped
            //std::vector<long>  m_linkJunctions;//Junction
            // How many pillars are cascaded with this one
            unsigned links = 0;

            Pillar(const Vec3d& endp, double h, double radius = 1.) :
                height{ h }, r(radius), endpt(endp), starts_from_head(false) {treeNodeType = MeshTypeElement::Pillar; }
            Pillar()
            {
                treeNodeType = MeshTypeElement::Pillar;
                id = ID_UNSET;

            }
            Vec3d startpoint() const
            {
                return { endpt.x(), endpt.y(), endpt.z() + height };
            }

            const Vec3d& endpoint() const { return endpt; }


        };

        // A wrapper struct around the pad
        struct Pad {
            TriangleMesh tmesh;
            PadConfig cfg;
            double zlevel = 0;
            bool showEnable = true;
            Pad() = default;

            Pad(const TriangleMesh& support_mesh,
                const ExPolygons& model_contours,
                double              ground_level,
                const PadConfig& pcfg,
                ThrowOnCancel       thr);

            bool empty() const { return tmesh.facets_count() == 0; }
        };
        // A base for pillars or bridges that end on the ground
        struct SquarePedestal : public SupportTreeNode {
            Vec3d pos;
            double height, r_bottom, r_top;

            SquarePedestal(const Vec3d& p, double h, double rbottom, double rtop)
                : pos{ p }, height{ h }, r_bottom{ rbottom }, r_top{ rtop }
            {treeNodeType = MeshTypeElement::SquarePedestal; }
            SquarePedestal()
            {
                treeNodeType = MeshTypeElement::SquarePedestal;
                id = ID_UNSET;

            }
        };


	}
}

#endif // SLIC3R_SUPPORTTREENODE_1637567374311_H
