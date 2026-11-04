# Apache License 2.0

import pytest

from ostk.physics.coordinate import Frame

from ostk.simulation import Component
from ostk.simulation import ComponentConfiguration


@pytest.fixture
def parent_component() -> Component:
    return Component(
        id="parent-id",
        name="Parent",
        type=Component.Type.Assembly,
        tags=[],
        geometries=[],
        components=[],
        parent=None,
        frame=Frame.GCRF(),
        simulator=None,
    )


class TestComponent:
    def test_constructor_with_type_none(self):
        component: Component = Component(
            id="component-id",
            name="Component",
            type=None,
            tags=[],
            geometries=[],
            components=[],
            parent=None,
            frame=Frame.GCRF(),
            simulator=None,
        )

        assert component.get_type() is None

    def test_get_type(self, parent_component: Component):
        assert parent_component.get_type() == Component.Type.Assembly

    def test_configure_without_type(self, parent_component: Component):
        for configuration in (
            ComponentConfiguration(id="camera-id", name="Camera"),
            ComponentConfiguration(id="camera-id", name="Camera", type=None),
        ):
            component: Component = Component.configure(configuration, parent_component)

            assert component.get_type() is None

    def test_configure_with_type(self, parent_component: Component):
        component: Component = Component.configure(
            ComponentConfiguration(
                id="camera-id",
                name="Camera",
                type=Component.Type.Sensor,
            ),
            parent_component,
        )

        assert component.get_type() == Component.Type.Sensor

    def test_type_has_no_undefined(self):
        assert not hasattr(Component.Type, "Undefined")
        assert Component.string_from_type(Component.Type.Sensor) == "Sensor"
