vec2 parallax(sampler2D depthMap, vec2 uv, vec3 viewDir, float heightScale, float minLayers, float maxLayers) {
    float numLayers = mix(maxLayers, minLayers, max(dot(vec3(0.0, 0.0, 1.0), viewDir), 0.0));
    // calculate the size of each layer
    float layerDepth = 1.0 / numLayers;
    // depth of current layer
    float currentLayerDepth = 0.0;
    // the amount to shift the texture coordinates per layer (from vector P)
    vec2 P = viewDir.xy * heightScale;
    vec2 deltaUV = P / numLayers;
    // get initial values
    vec2 currentUV = uv;
    float currentDepthMapValue = texture(depthMap, currentUV).r;

    while (currentLayerDepth < currentDepthMapValue) {
        // shift texture coordinates along direction of P
        currentUV -= deltaUV;
        // get depthmap value at current texture coordinates
        currentDepthMapValue = texture(depthMap, currentUV).r;
        // get depth of next layer
        currentLayerDepth += layerDepth;
    }

    // get texture coordinates before collision (reverse operations)
    vec2 prevUV = currentUV + deltaUV;

    // get depth after and before collision for linear interpolation
    float afterDepth  = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = texture(depthMap, prevUV).r - currentLayerDepth + layerDepth;

    // interpolation of texture coordinates
    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalUV = prevUV * weight + currentUV * (1.0 - weight);

    return finalUV;
}
