#include <iostream>
#include <vector>

#include "CGL/vector2D.h"

#include "mass.h"
#include "rope.h"
#include "spring.h"

namespace CGL {

    Rope::Rope(Vector2D start, Vector2D end, int num_nodes, float node_mass, float k, vector<int> pinned_nodes)
    {
        // TODO (Part 1): Create a rope starting at `start`, ending at `end`, and containing `num_nodes` nodes.

//        Comment-in this part when you implement the constructor
//        for (auto &i : pinned_nodes) {
//            masses[i]->pinned = true;
//        }
        if(num_nodes == 0 || num_nodes == 1)
            return ;

        Vector2D cur_pos = start;
        Vector2D prev_pos;

        Mass* cur_mass = new Mass(start,node_mass,false);
        Mass* prev_mass = nullptr;
        masses.push_back(cur_mass);

        Spring* cur_spr = nullptr;

        for(int i = 1; i < num_nodes ;i++)
        {
            if(i == num_nodes - 1)
            {
                cur_pos = end;
            }
            else
            {
                cur_pos = start + i * (end - start) / (num_nodes - 1.0) ;
            }

            prev_mass = cur_mass;

            cur_mass = new Mass(cur_pos, node_mass, false);
            cur_spr = new Spring(prev_mass, cur_mass, k);

            masses.push_back(cur_mass);
            springs.push_back(cur_spr);
        }   

        for (auto &i : pinned_nodes) {
            masses[i]->pinned = true;
        }

    }

    void Rope::simulateEuler(float delta_t, Vector2D gravity)
    {
        for (auto &s : springs)
        {
            // TODO (Part 2): Use Hooke's law to calculate the force on a node
            Vector2D len = s->m2->position - s->m1->position;
            Vector2D f = s->k * (len / len.norm() * (len.norm() - s->rest_length));
            s->m1->forces += f;
            s->m2->forces -= f;

            float k_d = 500;
            Vector2D velocity_gap = s->m2->velocity - s->m1->velocity;
            Vector2D f_d = k_d * dot(len / len.norm(), velocity_gap) * len / len.norm();
            s->m1->forces += f_d;
            s->m2->forces -= f_d;
        }

        for (auto &m : masses)
        {
            if (!m->pinned)
            {
                // TODO (Part 2): Add the force due to gravity, then compute the new velocity and position
                m->forces += gravity * m->mass;
                // TODO (Part 2): Add global damping
                
                float kd_global = 0.02;
                m->forces -= kd_global * m->velocity;

                Vector2D a = m->forces/m->mass;
                m->position += m->velocity * delta_t;
                m->velocity += a * delta_t;
                
            }

            // Reset all forces on each mass
            m->forces = Vector2D(0, 0);
        }
    }

    void Rope::simulateVerlet(float delta_t, Vector2D gravity)
    {
        for (auto &s : springs)
        {
            // TODO (Part 3): Simulate one timestep of the rope using explicit Verlet （solving constraints)
            Vector2D len = s->m2->position - s->m1->position;
            Vector2D f = s->k * (len/len.norm()) * (len.norm() - s->rest_length);
            s->m1->forces += f;
            s->m2->forces -= f;
        }

        for (auto &m : masses)
        {
            if (!m->pinned)
            {
                m->forces += gravity * m->mass;
                Vector2D a = m->forces / m->mass;

                Vector2D temp_position = m->position;
                // TODO (Part 3.1): Set the new position of the rope mass
                // TODO (Part 4): Add global Verlet damping

                float factor = 0.000001;
                m->position = m->position + ( 1 - factor) * (m->position - m->last_position) + a * delta_t * delta_t;
                m->last_position = temp_position;
            }
            m->forces = Vector2D(0,0);
        }
    }
}
