#ifndef ROUTEGRAPHVISUALIZER_H_
#define ROUTEGRAPHVISUALIZER_H_

namespace osg
{
	class Group;
}

namespace eva
{
	class RouteGraph;

	class RouteGraphVisualizer
	{
		public:
			virtual ~RouteGraphVisualizer(){};

			static osg::Group* drawRouteGraph(const RouteGraph& graph);

		private:
			RouteGraphVisualizer(){};
	};

}

#endif /* ROUTEGRAPHVISUALIZER_H_ */
