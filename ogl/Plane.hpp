
#ifndef _OGL_PLANE_
#define _OGL_PLANE_


/**
 * An OpenGL square plane.
 * Renders a flat quad of a particular size,
 * with grid lines along unit intervals.
 * The plane is always centered at the origin in world cooridnates.
 */
class Plane {

public:
	enum PLANE_WHICH {
		x,
		y,
		z
	};

private:

	PLANE_WHICH plane = PLANE_WHICH::x;

	glm::vec4 color = glm::vec4(0.20f, 0.20f, 0.20f, 1.0f);

	GLfloat size;

	bool softEdge;
	bool gridOnly;

	int numVertsGrid;

	GLuint programID;

	GLuint MVPID;

	GLuint vertexArrayID;
	GLuint vertBuffer;
	GLuint colorBuffer;

	void setupVAO() {
		glGenVertexArrays(1, &vertexArrayID);
		glBindVertexArray(vertexArrayID);

		GLfloat *verts;
		numVertsGrid = size * 2;
		GLint numCoords = gridOnly ? numVertsGrid * 3 : numVertsGrid * 3 + 4 * 3;
		verts = (GLfloat*)malloc(sizeof(GLfloat) * numCoords);
		GLint numColourInds = numCoords / 3 * 4;
		GLfloat *colours = (GLfloat*)malloc(sizeof(GLfloat) * numColourInds);
		for (int x = 0; x < numColourInds; x += 4) { // set colours except alpha
			for (int i = 0; i < 3; i++) {
				colours[x + i] = color[i];
			}
		}

		if (plane == PLANE_WHICH::x) {
			int vertsInd = 0;
			if (softEdge) {
				int coloursInd = 3;

				// zero out all the y coords
				for (int x = 1; x < numVertsGrid; x += 3) {
					verts[x] = 0.0f;
				}

				for (int i = -size; i < size; ++i) {
					colours[coloursInd] = (size-fabs(i))/size;
					coloursInd += 4;
					verts[vertsInd] = 1.0f*i;
					verts[vertsInd + 2] = 0.0f;
					vertsInd += 3;

					colours[coloursInd] = 0.0f;
					coloursInd += 4;
					verts[vertsInd] = 1.0f*i;
					verts[vertsInd + 2] = size;
					vertsInd += 3;

					colours[coloursInd] = (size-fabs(i))/size;
					coloursInd += 4;
					verts[vertsInd] = 1.0f*i;
					verts[vertsInd + 2] = 0.0f;
					vertsInd += 3;

					colours[coloursInd] = 0.0f;
					coloursInd += 4;
					verts[vertsInd] = 1.0f*i;
					verts[vertsInd + 2] = -size;
					vertsInd += 3;

					colours[coloursInd] = (size-fabs(i))/size;
					coloursInd += 4;
					verts[vertsInd] = 0.0f;
					verts[vertsInd + 2] = 1.0f*i;
					vertsInd += 3;

					colours[coloursInd] = 0.0f;
					coloursInd += 4;
					verts[vertsInd] = size;
					verts[vertsInd + 2] = 1.0f*i;
					vertsInd += 3;

					colours[coloursInd] = (size-fabs(i))/size;
					coloursInd += 4;
					verts[vertsInd] = 0.0f;
					verts[vertsInd + 2] = 1.0f*i;
					vertsInd += 3;

					colours[coloursInd] = 0.0f;
					coloursInd += 4;
					verts[vertsInd] = -size;
					verts[vertsInd + 2] = 1.0f*i;
					vertsInd += 3;
				}
			} else {
				for (int x = 0; x < numColourInds; x += 4) { // set alpha of colours
					colours[x] = color.w;
				}

				for (int i = -size; i < -size/2; ++i) {
					GLfloat tempVerts[] = {
						1.0f, 0.0f, -size,
						1.0f, 0.0f, size,
						size, 0.0f, 1.0f*i,
						-size, 0.0f, 1.0f*i
					};
					for (int x = 0; x < sizeof(tempVerts); x++) {
						verts[vertsInd + x] = tempVerts[x];
					}
					vertsInd += sizeof(tempVerts);
				}
				for (int i = size/2; i < size; ++i) {
					GLfloat tempVerts[] = {
						1.0f*i, 0.0f, -size,
						1.0f*i, 0.0f, size,
						size, 0.0f, 1.0f*i,
						-size, 0.0f, 1.0f*i
					};
					for (int x = 0; x < sizeof(tempVerts); x++) {
						verts[vertsInd + x] = tempVerts[x];
					}
					vertsInd += sizeof(tempVerts);
				}
			}

			if (!gridOnly) {
				for (int x = numColourInds - 4*4; x < numColourInds; x += 4) { // set colour (only need to set alpha), TODO: check logic
					colours[x] = color.w;
				}

				static const GLfloat planeVerts[] = {
					-size, 0.0f, -size,
					size, 0.0f, -size,
					size, 0.0f, size,
					-size, 0.0f, size
				};

				memcpy(verts + numVertsGrid, planeVerts, 4 * 3 * sizeof(int)); // TODO: check logic
			}
		}

		glGenBuffers(1, &vertBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertBuffer);
		glBufferData(GL_ARRAY_BUFFER, numCoords, verts, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(
			0,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*) 0                        // array buffer offset
		);

		glGenBuffers(1, &colorBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
		glBufferData(GL_ARRAY_BUFFER, numColourInds, colours, GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*) 0                        // array buffer offset
		);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void setup() {
		programID = LoadShaders( "shaders/ColorVertexShader.vertexshader", "shaders/ColorFragmentShader.fragmentshader");
		MVPID = glGetUniformLocation(programID, "MVP");
		glUseProgram(programID);

		setupVAO();
	}

public:

	/**
	 * Construct a default plane.
	 */
	Plane() : size(30.0), softEdge(false), gridOnly(false) { setup(); }


	/**
	 * Construct a plane of a particular size.
	 * @param sz, the side length of the plane.
	 */
	Plane(GLfloat sz) : size(sz), softEdge(false) { setup(); }


	/**
	 * Construct a plane of a particular size, with the option
	 * for a "soft edge" where the edge of the plane fades to 0 opacity.
	 *
	 * @param sz, the side length of the plane.
	 * @param softEdge_in, if true, edges of the plane fade to 0 opacity.
	 */
	Plane(GLfloat sz, bool softEdge_in) : size(sz), softEdge(softEdge_in) { setup(); }


	/**
	 * Construct a plane of a particular size, with the option
	 * for a "soft edge" where the edge of the plane fades to 0 opacity
	 * as well as the option to draw only grid lines.
	 *
	 * @param sz, the side length of the plane.
	 * @param softEdge_in, if true, edges of the plane fade to 0 opacity.
	 * @param gridOnly_in, if true, only the grid lines are drawn and not the plane itself.
	 */
	Plane(GLfloat sz, bool softEdge_in, bool gridOnly_in) : size(sz), softEdge(softEdge_in), gridOnly(gridOnly_in) { setup(); }


	/**
	 * Draw the plane.
	 */
	void draw(glm::mat4 V, glm::mat4 P) { // TODO: M var?
		glm::mat4 MVP = P*V; // TODO: *M?
		glBindVertexArray(vertexArrayID);
		glUseProgram(programID);

		glUniformMatrix4fv(MVPID, 1, GL_FALSE, &MVP[0][0]);

		glDrawArrays(GL_TRIANGLE_STRIP, numVertsGrid, 4);

		glLineWidth(0.5f);
		glDrawArrays(GL_LINES, 0, numVertsGrid);
		glLineWidth(1.0f);

		glUseProgram(0);
		glBindVertexArray(0);
	}

};

#endif
