
#ifndef _OGL_AXES_
#define _OGL_AXES_

#include "shader.hpp"

/**
 * An OpenGL set of axes.
 */
class Axes { // TODO: make sure nothing that shouldn't be exposed is exposed

	glm::vec3 origin;
	glm::vec3 extents;
	bool includeNeg;

	float endcapSize = 0.075;
	glm::vec4 xcol = glm::vec4(1.0f, 0.0f, 0.0f, 0.15f);
	glm::vec4 ycol = glm::vec4(0.0f, 1.0f, 0.0f, 0.15f);
	glm::vec4 zcol = glm::vec4(0.25f, 0.25f, 1.0f, 0.4f);
	glm::vec4 negxcol = glm::vec4(0.5f, 0.0f, 0.0f, 0.15f);
	glm::vec4 negycol = glm::vec4(0.0f, 0.5f, 0.0f, 0.15f);
	glm::vec4 negzcol = glm::vec4(0.15f, 0.15f, 0.5f, 0.4f);

	GLuint programID;

	GLuint MVPID;

	GLuint vertexArrayID;
	GLuint vertBuffer;
	GLuint colorBuffer;

	void setupVAO() {
		glGenVertexArrays(1, &vertexArrayID);
		glBindVertexArray(vertexArrayID);

		// vertices
		static const GLfloat verts[] = {
			origin.x, origin.y, origin.z,
			origin.x + extents.x, origin.y, origin.z,
			origin.x, origin.y, origin.z,
			origin.x, origin.y + extents.y, origin.z,
			origin.x, origin.y, origin.z,
			origin.x, origin.y, origin.z + extents.z,
		};

		glGenBuffers(1, &vertBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(
			0,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*) 0                        // array buffer offset
		);

		// colours
		static const GLfloat colours[] = {
			xcol.x, xcol.y, xcol.z, xcol.a,
			xcol.x, xcol.y, xcol.z, xcol.a,
			ycol.x, ycol.y, ycol.z, ycol.a,
			ycol.x, ycol.y, ycol.z, ycol.a,
			zcol.x, zcol.y, zcol.z, zcol.a,
			zcol.x, zcol.y, zcol.z, zcol.a,
		};

		glGenBuffers(1, &colorBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(colours), colours, GL_STATIC_DRAW);
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
	 * Construct a set of positive axes placed with their origin at orig in world space,
	 * each axis having a length specified by their respective dimension in ex.
	 *
	 * @param orig, the world coordinates of the axes's origin.
	 * @param ex, the extents of the three axes.
	 */
	Axes(glm::vec3 orig, glm::vec3 ex) : origin(orig), extents(ex), includeNeg(false) { setup(); }


	/**
	 * Construct a set of axes placed with their origin at orig in world space,
	 * each axis having a length specified by their respective dimension in ex.
	 *
	 * @param orig, the world coordinates of the axes's origin.
	 * @param ex, the extents of the three axes.
	 * @param posAndNeg, if true, draws negative axes as well.
	 */
	Axes(glm::vec3 orig, glm::vec3 ex, bool posAndNeg) : origin(orig), extents(ex), includeNeg(posAndNeg) { setup(); }

	/**
	 * Draw the axes.
	 */
	void draw(glm::mat4 V, glm::mat4 P) { // TODO: consolidate the changes between legacy + modern code (e.g. drawing a pt (?), endcapSize), need M var?
		/*
		glColor4f(xcol.x, xcol.y, xcol.z, xcol.a);
		glVertex3f(origin.x, origin.y, origin.z);
		glVertex3f(origin.x + extents.x, origin.y, origin.z);
		glVertex3f(origin.x + extents.x, origin.y, origin.z);
		glVertex3f(origin.x + extents.x, origin.y, origin.z+endcapSize);
		glVertex3f(origin.x + extents.x, origin.y, origin.z);
		glVertex3f(origin.x + extents.x, origin.y, origin.z-endcapSize);

		if (includeNeg) {
			glColor4f(negxcol.x, negxcol.y, negxcol.z, negxcol.a);
			glVertex3f(origin.x, origin.y, origin.z);
			glVertex3f(origin.x - extents.x, origin.y, origin.z);
			glVertex3f(origin.x - extents.x, origin.y, origin.z);
			glVertex3f(origin.x - extents.x, origin.y, origin.z+endcapSize);
			glVertex3f(origin.x - extents.x, origin.y, origin.z);
			glVertex3f(origin.x - extents.x, origin.y, origin.z-endcapSize);
		}

		glColor4f(ycol.x, ycol.y, ycol.z, ycol.a);
		glVertex3f(origin.x, origin.y, origin.z);
		glVertex3f(origin.x, origin.y + extents.y, origin.z);
		glVertex3f(origin.x, origin.y + extents.y, origin.z);
		glVertex3f(origin.x, origin.y + extents.y, origin.z+endcapSize);
		glVertex3f(origin.x, origin.y + extents.y, origin.z);
		glVertex3f(origin.x, origin.y + extents.y, origin.z-endcapSize);

		if (includeNeg) {
			glColor4f(negycol.x, negycol.y, negycol.z, negycol.a);
			glVertex3f(origin.x, origin.y, origin.z);
			glVertex3f(origin.x, origin.y - extents.y, origin.z);
			glVertex3f(origin.x, origin.y - extents.y, origin.z);
			glVertex3f(origin.x, origin.y - extents.y, origin.z+endcapSize);
			glVertex3f(origin.x, origin.y - extents.y, origin.z);
			glVertex3f(origin.x, origin.y - extents.y, origin.z-endcapSize);
		}

		glColor4f(zcol.x, zcol.y, zcol.z, zcol.a);
		glVertex3f(origin.x, origin.y, origin.z);
		glVertex3f(origin.x, origin.y, origin.z + extents.z);
		glVertex3f(origin.x, origin.y, origin.z + extents.z);
		glVertex3f(origin.x+endcapSize, origin.y, origin.z + extents.z);
		glVertex3f(origin.x, origin.y, origin.z + extents.z);
		glVertex3f(origin.x-endcapSize, origin.y, origin.z + extents.z);

		if (includeNeg) {
			glColor4f(negzcol.x, negzcol.y, negzcol.z, negzcol.a);
			glVertex3f(origin.x, origin.y, origin.z);
			glVertex3f(origin.x, origin.y, origin.z - extents.z);
			glVertex3f(origin.x, origin.y, origin.z - extents.z);
			glVertex3f(origin.x+endcapSize, origin.y, origin.z - extents.z);
			glVertex3f(origin.x, origin.y, origin.z - extents.z);
			glVertex3f(origin.x-endcapSize, origin.y, origin.z - extents.z);
		}
		*/

		glm::mat4 MVP = P*V; // TODO: *M?
		glBindVertexArray(vertexArrayID);
		glUseProgram(programID);

		glUniformMatrix4fv(MVPID, 1, GL_FALSE, &MVP[0][0]);

		glLineWidth(3.0f);
		glDrawArrays(GL_LINES, 0, 6);
		glLineWidth(1.0f);

		glUseProgram(0);
		glBindVertexArray(0);
	}

};


#endif
