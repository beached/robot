/*
 *	(c)2012 Darrell Wright
 *	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef HAS_DAWCONCURRENTQUEUE_HEADER
#define HAS_DAWCONCURRENTQUEUE_HEADER

#include <condition_variable>
#include <queue>
#include <mutex>

namespace daw { namespace concurrency {

	template<class data_t> class concurrent_queue {
	private:
		std::queue<data_t> m_queue;
		std::mutex m_mutex;
		std::condition_variable m_condition_variable;
	public:
		void push( data_t const& data ) {
			std::unique_lock<std::mutex> lock( m_mutex );
			m_queue.push( data );
			lock.unlock( );
			m_condition_variable.notify_one( );
		}

		bool empty( ) const {
			std::unique_lock<std::mutex> lock( m_mutex );
			return m_queue.empty( );
		}

		size_t size( ) const {
			std::unique_lock<std::mutex> lock( m_mutex );
			return m_queue.size( );
		}

		bool try_pop( data_t& popped_value ) {
			std::unique_lock<std::mutex> lock( m_mutex );
			if( m_queue.empty( ) ) {
				return false;
			}

			popped_value = m_queue.front( );
			m_queue.pop( );
			return true;
		}

		void wait_and_pop( data_t& popped_value ) {
			std::unique_lock<std::mutex> lock( m_mutex );
			while( m_queue.empty( ) ) {
				m_condition_variable.wait( lock );
			}

			popped_value = m_queue.front( );
			m_queue.pop( );
		}
	};

}}

#endif	//HAS_DAWCONCURRENTQUEUE_HEADER

