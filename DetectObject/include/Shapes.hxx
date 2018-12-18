#include <opencv2/core/types.hpp>

#include <algorithm>

namespace Shape
{
	template <typename Type = std::float_t, typename = std::enable_if_t<std::is_arithmetic_v<Type>>>
	struct Circle
	{
	 private:
		Type m_center_x;
		Type m_center_y;
		Type m_radius;

	 public:
		inline Circle(const Type p_center_x, const Type p_center_y, const decltype(m_radius) p_radius) noexcept :
			 m_center_x{p_center_x}, m_center_y{p_center_y}, m_radius{std::abs(p_radius)}
		{
		}
		inline Circle(const std::vector<cv::Point>& p_circle_enclosing)
		{
			float			radius;
			cv::Point2f center;

			cv::minEnclosingCircle(p_circle_enclosing, center, radius);

			setCenter(center);
			setRadius(radius);
		}
		inline Circle(std::nullptr_t) : Circle(0, 0, 0u) {}
		template <typename UserType,
					 typename = std::enable_if_t<std::is_arithmetic_v<UserType> &&
														  // Ensure that when Integer Casted, the ranges are different
														  // So that we don't loose data
														  (std::numeric_limits<Type>::max() >= std::numeric_limits<UserType>::max())>>
		inline Circle(const cv::Vec<UserType, 3>& params) :
			 Circle(params[0 /*Center X*/], params[1 /*Center Y*/], std::abs(params[2 /*Center Z*/]))
		{
		}
		template <typename UserType,
					 typename = std::enable_if_t<std::is_arithmetic_v<UserType> &&
														  // Ensure that when Integer Casted, the ranges are different
														  // So that we don't loose data
														  (std::numeric_limits<Type>::max() >= std::numeric_limits<UserType>::max())>>
		inline Circle(const cv::Point_<UserType>& p_center, const decltype(m_radius) p_radius) noexcept :
			 Circle(p_center.x, p_center.y, p_radius)
		{
		}
		inline auto getCenter() const
		{
			return cv::Point_<Type>{getCenterX(), getCenterY()};
		}
		inline Type getCenterX() const noexcept
		{
			return m_center_x;
		}
		inline Type getCenterY() const noexcept
		{
			return m_center_y;
		}
		inline auto getRadius() const noexcept
		{
			return m_radius;
		}

		template <typename UserType,
					 typename = std::enable_if_t<std::is_arithmetic_v<UserType> &&
														  // Ensure that when Integer Casted, the ranges are different
														  // So that we don't loose data
														  (std::numeric_limits<Type>::max() >= std::numeric_limits<UserType>::max())>>
		inline Circle& setCenter(const cv::Point_<UserType>& p_center) noexcept
		{
			return setCenter(p_center.x, p_center.y);
		}
		template <typename UserType,
					 typename = std::enable_if_t<std::is_arithmetic_v<UserType> &&
														  // Ensure that when Integer Casted, the ranges are different
														  // So that we don't loose data
														  (std::numeric_limits<Type>::max() >= std::numeric_limits<UserType>::max())>>
		inline Circle& setCenter(const UserType p_center_x, const UserType p_center_y) noexcept
		{
			m_center_x = p_center_x;
			m_center_y = p_center_y;

			return *this;
		}
		inline Circle& setRadius(const decltype(m_radius) p_radius) noexcept
		{
			m_radius = p_radius;
			return *this;
		}

		bool empty() const noexcept
		{
			return m_radius == 0;
		}
	};
} // namespace Shape