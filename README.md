Gloss
======

A [bidirectional path tracer](http://en.wikipedia.org/wiki/Path_tracing#Bidirectional_path_tracing) written in object oriented C.

Blog Posts
----------

[Gloss - My Bidirectional Path Tracer](http://geon.github.io/programming/2013/08/22/gloss-my-bidirectional-path-tracer/)

TODO
----

* Materials
	* Seems I had misunderstood diffuse relfection: http://en.wikipedia.org/wiki/Diffuse_reflection
		The BRDF function is correct, dot(normal, lightDir)
		But the importance sampling whould prioritize the normal of the surface, and drop to 0 at the tangent, just like the BRDF.
	* Fresnel term
	* Texturing
		* Procedural
		* Bitmapped
	* Refraction
		* Dispersion
	* "Directionality" of emitted light
* Recursive geometry - fractals
* Better camera model
	* Depth Of Field
	* Lens flare / bloom
	* Tone mapping