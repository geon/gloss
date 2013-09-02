Gloss
======

A [bidirectional path tracer](http://en.wikipedia.org/wiki/Path_tracing#Bidirectional_path_tracing) written in object oriented C.

Blog Posts
----------

* [Gloss - My Bidirectional Path Tracer](http://geon.github.io/programming/2013/08/22/gloss-my-bidirectional-path-tracer/)
* [Object Oriented C](http://geon.github.io/programming/2013/09/01/object-oriented-c)
* [Restructured Code And Glossy Reflections](http://geon.github.io/programming/2013/09/01/restructured-code-and-glossy-reflections)
* [Generating Tangents From A Normal](http://geon.github.io/programming/2013/09/02/generating-tangents-from-a-normal/)

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