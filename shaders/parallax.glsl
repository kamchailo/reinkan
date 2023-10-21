// Parallax Occlusion Funcitons

ivec2 GetTileDifferent (vec2 uvA, vec2 uvB, int maxLevel, int level)
{
    float denominator = pow(2, maxLevel - level);

    ivec2 tileA = ivec2(uvA / denominator);
    ivec2 tileB = ivec2(uvB / denominator);

    return tileA - tileB;
}

vec3 StopAtTileBorder(vec3 pPrime, vec3 pTemp, int maxLevel, int level)
{
    float denominator = pow(2, maxLevel - level);
    ivec2 tilePPrime = ivec2(pPrime / denominator);
    float scale;
    vec3 direction = pTemp - pPrime;
    if(abs(direction.x) > abs(direction.y))
    {
        float borderX;
        if(direction.x > 0)
        {
            borderX = (tilePPrime.x + 1) * denominator;
        }
        else
        {
            borderX = (tilePPrime.x - 1) * denominator;
        }
        
        scale = borderX / pPrime.x;
    }
    else // abs(y) > abs(x)
    {
        float borderY;
        if(direction.y > 0)
        {
            borderY = (tilePPrime.y + 1) * denominator;
        }
        else
        {
            borderY = (tilePPrime.y - 1) * denominator;
        }
        
        scale = borderY / pPrime.y;
    }

    return  vec3(pPrime.x * scale, pPrime.y * scale, pPrime.z * scale);
}
