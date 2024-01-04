//
// Created by Göksu Güvendiren on 2019-05-14.
//

#include "Scene.hpp"


void Scene::buildBVH() {
    printf(" - Generating BVH...\n\n");
    this->bvh = new BVHAccel(objects, 1, BVHAccel::SplitMethod::NAIVE);
}

Intersection Scene::intersect(const Ray &ray) const
{
    return this->bvh->Intersect(ray);
}

void Scene::sampleLight(Intersection &pos, float &pdf) const
{
    float emit_area_sum = 0;
    for (uint32_t k = 0; k < objects.size(); ++k) {
        if (objects[k]->hasEmit()){
            emit_area_sum += objects[k]->getArea();
        }
    }
    float p = get_random_float() * emit_area_sum;
    emit_area_sum = 0;
    for (uint32_t k = 0; k < objects.size(); ++k) {
        if (objects[k]->hasEmit()){
            emit_area_sum += objects[k]->getArea();
            if (p <= emit_area_sum){
                objects[k]->Sample(pos, pdf);
                break;
            }
        }
    }
}

bool Scene::trace(
        const Ray &ray,
        const std::vector<Object*> &objects,
        float &tNear, uint32_t &index, Object **hitObject)
{
    *hitObject = nullptr;
    for (uint32_t k = 0; k < objects.size(); ++k) {
        float tNearK = kInfinity;
        uint32_t indexK;
        Vector2f uvK;
        if (objects[k]->intersect(ray, tNearK, indexK) && tNearK < tNear) {
            *hitObject = objects[k];
            tNear = tNearK;
            index = indexK;
        }
    }


    return (*hitObject != nullptr);
}

// Implementation of Path Tracing
Vector3f Scene::castRay(const Ray &ray, int depth) const
{
    // TO DO Implement Path Tracing Algorithm here
    Vector3f radiance = Vector3f(0.0, 0.0, 0.0);

    // Find closest ray-object intersection
    Intersection intersection = Scene::intersect(ray);

    if (intersection.happened)
    {
        // Check if the intersection is with a light source

        // If the intersection is with a light source, return the emission
        if (intersection.m->hasEmission())
        {
            radiance = intersection.m->getEmission();
            return radiance;
        }

        // If the intersection is not with a light source, then compute the radiance
        else
        {
            // Initialize direct light and indirect light
            Vector3f L_dir = Vector3f(0.0, 0.0, 0.0);
            Vector3f L_indir = Vector3f(0.0, 0.0, 0.0);

            // Sample light source
            Intersection light_sample;
            float pdf_light;
            sampleLight(light_sample, pdf_light);

            // Check if the ray is blocked by other objects
            Vector3f itersection_origin = intersection.coords + intersection.normal * EPSILON;
            Vector3f obj_light_direction = normalize(light_sample.coords - intersection.coords);
            Ray obj_light_ray = Ray(itersection_origin, obj_light_direction);
            Intersection obj_light_intersection = Scene::intersect(obj_light_ray);

            // If the ray is not blocked, then compute the direct light
            if (obj_light_intersection.happened && obj_light_intersection.m->hasEmission())
            {
                // [comment]
                // Compute the direct light
                // formula: L_dir = L_i * BRDF * cos(theta) * cos(theta') / ||x' - p||^2 / pdf
                // L_i: light intensity
                // BRDF or f_r: material BRDF
                // cos(theta): cosine of the angle between the light direction and the normal at the intersection point
                // cos(theta'): cosine of the angle between the light direction and the normal at the light source
                // ||x' - p||: distance between the light source and the intersection point
                // pdf: probability density function of the light source
                // [comment]

                // Compute light intensity
                Vector3f L_i = light_sample.emit;

                // Compute BRDF
                Vector3f light_dir = normalize(light_sample.coords - intersection.coords);
                Vector3f BRDF = intersection.m->eval(ray.direction, light_dir, intersection.normal);

                // Compute theta and theta'
                float cos_theta = dotProduct(intersection.normal, light_dir);
                float cos_theta_prime = dotProduct(light_sample.normal, -light_dir);

                // Compute distance between the light source and the intersection point
                float distance = (light_sample.coords - intersection.coords).norm();

                // Compute the radiance
                L_dir = L_i * BRDF * cos_theta * cos_theta_prime / pow(distance, 2) / pdf_light;
                radiance += L_dir;
            }

            // [comment]
            // Compute the indirect light
            // formula: L_indir = castRay(new_ray, depth + 1) * BRDF * cos(theta) / pdf_hemi / P_RR
            // [comment]

            // Test Russian Roulette
            float random = get_random_float();
            if (random > RussianRoulette)
            {
                return radiance;
            }

            // Compute new ray direction
            Vector3f new_ray_direction = intersection.m->sample(ray.direction, intersection.normal);

            // Compute BRDF
            Vector3f BRDF = intersection.m->eval(ray.direction, new_ray_direction, intersection.normal);

            // Compute theta
            float cos_theta = dotProduct(intersection.normal, new_ray_direction);

            // Compute the radiance
            Ray new_ray = Ray(intersection.coords + intersection.normal * EPSILON, new_ray_direction);

            // Compute pdf_hemi
            float pdf_hemi = intersection.m->pdf(ray.direction, new_ray_direction, intersection.normal);

            // Compute the radiance
            L_indir = castRay(new_ray, depth + 1) * BRDF * cos_theta / pdf_hemi / RussianRoulette;

            radiance += L_indir;
        }
    }

    return radiance;

}