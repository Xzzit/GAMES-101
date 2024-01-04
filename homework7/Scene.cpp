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
        if (intersection.m->hasEmission())
        {
            // If the intersection is with a light source, return the emission
            radiance = intersection.m->getEmission();
        }

        else
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

            // Check if the ray is blocked by other objects
            // If the ray is blocked, then the radiance is 0
            // If the ray is not blocked, then the radiance is the direct light
            
            
            // Sample light source
            Intersection light_sample;
            float pdf_light;
            sampleLight(light_sample, pdf_light);

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
            Vector3f L_dir = L_i * BRDF * cos_theta * cos_theta_prime / pow(distance, 2) / pdf_light;
            radiance += L_dir;

            // print
            // std::cout << "L_i: " << L_i << std::endl;
            // std::cout << "BRDF: " << BRDF << std::endl;
            // std::cout << "cos_theta: " << cos_theta << std::endl;
            // std::cout << "cos_theta_prime: " << cos_theta_prime << std::endl;
            // std::cout << "distance: " << distance << std::endl;
            // std::cout << "pdf_light: " << pdf_light << std::endl;
            // std::cout << "L_dir: " << L_dir << std::endl;
            // std::cout << "\n" << std::endl;
        }
    }

    return radiance;

}