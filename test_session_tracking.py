import unittest
from session_tracking import get_db_connection, create_sessions_table, insert_session_data, retrieve_session_data, handle_new_session

class TestSessionTracking(unittest.TestCase):
    
    def setUp(self):
        """Shared setup method to initialize the database and table."""
        self.conn = get_db_connection()
        self.cursor = self.conn.cursor()
        create_sessions_table(self.cursor)

    def tearDown(self):
        """Clean up method to delete data after each test."""
        self.cursor.execute('DELETE FROM sessions')
        self.conn.commit()

    def test_create_sessions_table(self):
        """Test if the sessions table is created."""
        self.assertTrue(self.cursor.execute('SELECT * FROM sessions').fetchone() is None)

    def test_insert_session_data(self):
        """Test if session data is inserted correctly."""
        insert_session_data(self.cursor, 1, 'reset_context=False, check_flags=False', 'Started the conversation with some context.', '2023-03-01 12:00:00')
        self.assertTrue(self.cursor.execute('SELECT * FROM sessions').fetchone() is not None)

    def test_retrieve_session_data(self):
        """Test if session data is retrieved correctly."""
        insert_session_data(self.cursor, 1, 'reset_context=False, check_flags=False', 'Started the conversation with some context.', '2023-03-01 12:00:00')
        session_data = retrieve_session_data(self.cursor, 1)
        self.assertEqual(session_data[0], 'reset_context=False, check_flags=False')
        self.assertEqual(session_data[1], 'Started the conversation with some context.')

    def test_retrieve_non_existent_session(self):
        """Test retrieving a session that doesn't exist."""
        session_data = retrieve_session_data(self.cursor, 999)  # assuming 999 is an invalid ID
        self.assertIsNone(session_data)

    def test_handle_new_session_insert(self):
        """Test handling a new session insert."""
        handle_new_session(self.cursor, 1, 'reset_context=False, check_flags=False', 'Started the conversation with some context.')
        self.conn.commit()
        session_data = retrieve_session_data(self.cursor, 1)
        self.assertIsNotNone(session_data)
        self.assertEqual(session_data[0], 'reset_context=False, check_flags=False')
        self.assertEqual(session_data[1], 'Started the conversation with some context.')

    def test_handle_new_session_update(self):
        """Test handling a new session update."""
        handle_new_session(self.cursor, 1, 'reset_context=False, check_flags=False', 'Started the conversation with some context.')
        self.conn.commit()
        handle_new_session(self.cursor, 1, 'reset_context=True, check_flags=True', 'Updated the conversation with new context.')
        self.conn.commit()
        session_data = retrieve_session_data(self.cursor, 1)
        self.assertIsNotNone(session_data)
        self.assertEqual(session_data[0], 'reset_context=True, check_flags=True')
        self.assertEqual(session_data[1], 'Updated the conversation with new context.')

    def test_instructions(self):
        """Test if instructions are printed correctly."""
        from session_tracking import instructions
        instructions()

if __name__ == '__main__':
    unittest.main()
