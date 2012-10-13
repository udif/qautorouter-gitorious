#include "topoplugin.h"

PadType *TopoRouter::AddPad(toporouter_t *r, char *Name,
            gdouble P1X, gdouble P1Y, gdouble P2X, gdouble P2Y,gdouble Thickness,
            gdouble Radius, unsigned long Shape, int Layer)
{
    toporouter_spoint_t p[2], rv[5];
    gdouble x[2], y[2], t, m;
    GList *objectconstraints;
    toporouter_layer_t *l = &(r->layers[Layer]);
    GList *vlist = NULL;
    toporouter_bbox_t *bbox = NULL;
    PadType *NewPad = (PadType *)g_malloc(sizeof(PadType));
#warning Memory leak: This is never freed!
    objectconstraints = NULL;

    if(!NewPad)
    {
        printf("Could not allocate memory for pad\n");
        exit(1);
    }
    t = Thickness / 2.0f;
    x[0] = P1X;
    x[1] = P2X;
    y[0] = P1Y;
    y[1] = P2Y;

#warning Memory leak: String needs to be freed
    NewPad->Name = strdup(Name);
    if(!NewPad->Name)
    {
        printf("Could not allocate memory for string\n");
        exit(1);
    }
    NewPad->Thickness = Thickness;
//     NewPad->Type = Shape;
    NewPad->Point1.X = P1X;
    NewPad->Point1.Y = P1Y;
    NewPad->Point2.X = P2X;
    NewPad->Point2.Y = P2X;
//     NewPad->Origin.X = OriginX;
//     NewPad->Origin.Y = OriginY;
//     NewPad->Group = Layer;

    if(SQUAREFLAG == Shape)
    {
        /* Square or oblong pad. Four points and four constraint edges are
        * used */
        if(x[0] == x[1] && y[0] == y[1])
        {
            /* Pad is square */
            vlist = rect_with_attachments(Radius,
                x[0]-t, y[0]-t,
                x[0]-t, y[0]+t,
                x[0]+t, y[0]+t,
                x[0]+t, y[0]-t,
                Layer);

            bbox = toporouter_bbox_create(Layer, vlist, PAD, NewPad);
            r->bboxes = g_slist_prepend(r->bboxes, bbox);
            insert_constraints_from_list(r, l, vlist, bbox);

            g_list_free(vlist);

            bbox->point = GTS_POINT( insert_vertex(r, l, x[0], y[0], bbox) );
            g_assert(TOPOROUTER_VERTEX(bbox->point)->bbox == bbox);
        }
        else
        {
            /* Pad is diagonal oblong or othogonal oblong */

            m = cartesian_gradient(x[0], y[0], x[1], y[1]);

            p[0].x = x[0]; p[0].y = y[0];
            p[1].x = x[1]; p[1].y = y[1];

            vertex_outside_segment(&p[0], &p[1], t, &rv[0]);
            vertices_on_line(&rv[0], perpendicular_gradient(m), t, &rv[1], &rv[2]);

            vertex_outside_segment(&p[1], &p[0], t, &rv[0]);
            vertices_on_line(&rv[0], perpendicular_gradient(m), t, &rv[3], &rv[4]);

            if(wind(&rv[1], &rv[2], &rv[3]) != wind(&rv[2], &rv[3], &rv[4]))
            {
                rv[0].x = rv[3].x; rv[0].y = rv[3].y;
                rv[3].x = rv[4].x; rv[3].y = rv[4].y;
                rv[4].x = rv[0].x; rv[4].y = rv[0].y;
            }

            vlist = rect_with_attachments(Radius,
                rv[1].x, rv[1].y,
                rv[2].x, rv[2].y,
                rv[3].x, rv[3].y,
                rv[4].x, rv[4].y,
                Layer);


            bbox = toporouter_bbox_create(Layer, vlist, PAD, NewPad);
            r->bboxes = g_slist_prepend(r->bboxes, bbox);
            insert_constraints_from_list(r, l, vlist, bbox);
            g_list_free(vlist);

            printf("Pad %s: OBLONG : rect_with_attachments(%2.2f, %2.2f,  %2.2f,  %2.2f,  %2.2f,  %2.2f,  %2.2f,  %2.2f,  %2.2f,  %2.2f)\n",
                            NewPad->Name, (double)Radius,
                            (double)rv[1].x, (double)rv[1].y,
                            (double)rv[2].x, (double)rv[2].y,
                            (double)rv[3].x, (double)rv[3].y,
                            (double)rv[4].x, (double)rv[4].y,
                            (double)(Layer));

            //bbox->point = GTS_POINT( gts_vertex_new(vertex_class, (x[0] + x[1]) / 2., (y[0] + y[1]) / 2., 0.) );
            bbox->point = GTS_POINT( insert_vertex(r, l, (x[0] + x[1]) / 2., (y[0] + y[1]) / 2., bbox) );
            g_assert(TOPOROUTER_VERTEX(bbox->point)->bbox == bbox);
        }
    }
    else
    {
        /* Either round pad or pad with curved edges */

        if(x[0] == x[1] && y[0] == y[1])
        {
            /* One point */

            /* bounding box same as square pad */
            vlist = rect_with_attachments(Radius,
            x[0]-t, y[0]-t,
            x[0]-t, y[0]+t,
            x[0]+t, y[0]+t,
            x[0]+t, y[0]-t,
            l - r->layers);
            bbox = toporouter_bbox_create(l - r->layers, vlist, PAD, NewPad);
            r->bboxes = g_slist_prepend(r->bboxes, bbox);
            g_list_free(vlist);

            //bbox->point = GTS_POINT( insert_vertex(r, l, x[0], y[0], bbox) );
            bbox->point = GTS_POINT( insert_vertex(r, l, x[0], y[0], bbox) );

        }
        else
        {
            /* Two points and one constraint edge */

            /* the rest is just for bounding box */
            m = cartesian_gradient(x[0], y[0], x[1], y[1]);

            p[0].x = x[0]; p[0].y = y[0];
            p[1].x = x[1]; p[1].y = y[1];

            vertex_outside_segment(&p[0], &p[1], t, &rv[0]);
            vertices_on_line(&rv[0], perpendicular_gradient(m), t, &rv[1], &rv[2]);

            vertex_outside_segment(&p[1], &p[0], t, &rv[0]);
            vertices_on_line(&rv[0], perpendicular_gradient(m), t, &rv[3], &rv[4]);

            if(wind(&rv[1], &rv[2], &rv[3]) != wind(&rv[2], &rv[3], &rv[4]))
            {
                rv[0].x = rv[3].x; rv[0].y = rv[3].y;
                rv[3].x = rv[4].x; rv[3].y = rv[4].y;
                rv[4].x = rv[0].x; rv[4].y = rv[0].y;
            }

            vlist = rect_with_attachments(Radius,
            rv[1].x, rv[1].y,
            rv[2].x, rv[2].y,
            rv[3].x, rv[3].y,
            rv[4].x, rv[4].y,
            l - r->layers);
            bbox = toporouter_bbox_create(l - r->layers, vlist, PAD, NewPad);
            r->bboxes = g_slist_prepend(r->bboxes, bbox);
            insert_constraints_from_list(r, l, vlist, bbox);
            g_list_free(vlist);

            //bbox->point = GTS_POINT( gts_vertex_new(vertex_class, (x[0] + x[1]) / 2., (y[0] + y[1]) / 2., 0.) );
            bbox->point = GTS_POINT( insert_vertex(r, l, (x[0] + x[1]) / 2., (y[0] + y[1]) / 2., bbox) );

            //bbox->constraints = g_list_concat(bbox->constraints, insert_constraint_edge(r, l, x[0], y[0], x[1], y[1], bbox));
        }
    }
    return NewPad;
}